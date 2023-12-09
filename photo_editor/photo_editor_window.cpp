#include "photo_editor_window.h"
#include "./ui_photo_editor_window.h"
#include "save_dialog.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QString>
#include <QMessageBox>

PhotoEditorWindow::PhotoEditorWindow(QColor bgColor, QSize bgSize, QString name, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditorWindow)
    , projectName(name)
    , items{}
    , itemsCount{0}
{
    ui->setupUi(this);

    mainScene = new QGraphicsScene(this);
    mainScene->setSceneRect(0, 0, bgSize.width(), bgSize.height());

    ui->mainGraphView->setScene(mainScene);
    ui->mainGraphView->setMouseTracking(true);
    ui->mainGraphView->setBackgroundBrush(QBrush(Qt::gray));

    layer = new QBaseLayer(bgSize, bgColor);
    mainScene->addItem(layer);

    setWindowTitle(projectName + " - Photo editor");
}

PhotoEditorWindow::PhotoEditorWindow(QString filePath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditorWindow)
    , items{}
    , itemsCount{0}
{
    ui->setupUi(this);

    std::unique_ptr<QPhotoItem> item{std::make_unique<QPhotoItem>(filePath, items, itemsCount)};

    mainScene = new QGraphicsScene(this);
    mainScene->setSceneRect(0, 0, item->getSize().width(), item->getSize().height());

    ui->mainGraphView->setScene(mainScene);
    ui->mainGraphView->setMouseTracking(true);
    ui->mainGraphView->setBackgroundBrush(QBrush(Qt::gray));

    layer = new QBaseLayer(item->getSize(), QColor(255, 255, 255));
    mainScene->addItem(layer);

    setWindowTitle(item->getName() + " - Photo editor");
    projectName = item->getName();
    addItem(item, filePath);
}

PhotoEditorWindow::~PhotoEditorWindow()
{
    delete ui;
    delete mainScene;
    delete layer;
}

void PhotoEditorWindow::addItem(std::unique_ptr<QPhotoItem> &item, QString filePath)
{
    item->setParentItem(layer);
    item->setPos(layer->getSize().width() / 2 - item->getSize().width() / 2,
                 layer->getSize().height() / 2 - item->getSize().height() / 2);

    QListWidgetItem *listItem = new QListWidgetItem(QIcon(filePath), item->getName(), ui->listItems);
    listItem->setData(Qt::UserRole, QVariant(itemsCount));
    item->setViewUpdate(std::bind(&PhotoEditorWindow::updatePhotoView,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2));
    items.emplace(itemsCount++, std::move(item));
}

void PhotoEditorWindow::updatePhotoView(QPhotoItem &item, QPhotoItem::PhotoItemChanged change)
{
    switch (change) {
    case QPhotoItem::PhotoItemChanged::ItemSelectionChanged:
        for (int i = 0; i < ui->listItems->count(); i++) {
            if (ui->listItems->item(i)->data(Qt::UserRole).toInt() == item.getId()
                && (ui->listItems->item(i)->isSelected() != item.isSelected()))
                ui->listItems->item(i)->setSelected(item.isSelected());
        }
        break;
    default:
        break;
    }
}

void PhotoEditorWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        if (ui->listItems->selectedItems().size() > 0) {
            items.erase(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt());
            ui->listItems->takeItem(ui->listItems->row(ui->listItems->selectedItems()[0]));
        }
    }
    return QWidget::keyPressEvent(event);
}

void PhotoEditorWindow::on_btnAddItem_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Image", QDir::homePath() + "/Pictures/", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        auto item = std::make_unique<QPhotoItem>(filePath, items, itemsCount);
        addItem(item, filePath);
    }
}

void PhotoEditorWindow::on_listItems_itemSelectionChanged()
{
    if (!(ui->listItems->selectedItems().length() > 0
          && items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->isSelected())) {
        for (auto &el : items) {
            if (el.second->isSelected() == true) {
                el.second->setSelected(false);
                break;
            }
        }
        if (ui->listItems->selectedItems().length() > 0) {
            items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())
                ->setSelected(true);
        }
    }
}

void PhotoEditorWindow::on_btnSave_clicked()
{
    SaveDialog saveDialog(projectName);
    if (ui->listItems->selectedItems().size() > 0) {
        items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->setSelected(false);
    }
    if (saveDialog.exec()) {
        QImage image(layer->getSize(), QImage::Format_ARGB32);
        QPainter painter(&image);
        mainScene->render(&painter);
        painter.end();

        if (image.save(saveDialog.getFilePath(), nullptr, saveDialog.getQuality())) {
            QMessageBox::information(nullptr, "Success", "Your image was successfully saved!");
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to saved the image.");
        }
    }
}

void PhotoEditorWindow::on_pushButton_2_clicked()
{
    layer->scale(300, 300);
    mainScene->update();
}

