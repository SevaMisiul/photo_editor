#include "photo_editor_window.h"
#include "./ui_photo_editor_window.h"
#include "save_dialog.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QString>
#include <qdebug.h>

PhotoEditorWindow::PhotoEditorWindow(QColor bgColor, QSize bgSize, QString name, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditorWindow)
    , projectName(name)
    , items{}
    , listWidgetItems{}
    , itemsCount{0}
{
    ui->setupUi(this);

    mainScene = new QGraphicsScene(this);
    mainScene->setSceneRect(0, 0, bgSize.width(), bgSize.height());

    ui->mainGraphView->setScene(mainScene);
    ui->mainGraphView->setMouseTracking(true);
    ui->mainGraphView->setBackgroundBrush(QBrush(Qt::gray));

    layer = new QBaseLayer(bgSize, bgColor);
    layer->setViewUpdate(
        std::bind(&PhotoEditorWindow::updateLayerView, this, std::placeholders::_1));

    auto listWidgetItem = std::make_unique<QListWidgetItem>("Background", ui->listItems);
    listWidgetItem->setData(Qt::UserRole, QVariant(-1));
    listWidgetItems.emplace(-1, std::move(listWidgetItem));

    mainScene->addItem(layer);

    setWindowTitle(projectName + " - Photo editor");

    QIntValidator *posValidator = new QIntValidator(-9999, 9999);
    ui->edtX->setValidator(posValidator);
    ui->edtY->setValidator(posValidator);
    connect(ui->edtX,
            &QLineEdit::editingFinished,
            this,
            &PhotoEditorWindow::on_edtPos_editingFinished);
    connect(ui->edtY,
            &QLineEdit::editingFinished,
            this,
            &PhotoEditorWindow::on_edtPos_editingFinished);
    QIntValidator *sizeValidator = new QIntValidator(1, 9999);
    ui->edtWidth->setValidator(sizeValidator);
    ui->edtHeight->setValidator(sizeValidator);
    connect(ui->edtWidth,
            &QLineEdit::editingFinished,
            this,
            &PhotoEditorWindow::on_edtSize_editingFinished);
    connect(ui->edtHeight,
            &QLineEdit::editingFinished,
            this,
            &PhotoEditorWindow::on_edtSize_editingFinished);

    ui->gbSize->setVisible(false);
    ui->gbPos->setVisible(false);
    ui->gbTranspose->setVisible(false);
    ui->btnUp->setVisible(false);
    ui->gbFilters->setVisible(false);

    ui->cbFilters->addItem("------",
                           QVariant::fromValue(QPhotoItem::PhotoFilter::Null));
    ui->cbFilters->addItem("Monochrome",
                           QVariant::fromValue(QPhotoItem::PhotoFilter::Monochrome));
    ui->cbFilters->addItem("Sepia",
                           QVariant::fromValue(QPhotoItem::PhotoFilter::Sepia));
    ui->cbFilters->addItem("Negative",
                           QVariant::fromValue(QPhotoItem::PhotoFilter::Negative));
    ui->cbFilters->addItem("Retro",
                           QVariant::fromValue(QPhotoItem::PhotoFilter::Retro));
    ui->cbFilters->addItem("Noise",
                           QVariant::fromValue(QPhotoItem::PhotoFilter::Noise));
}

PhotoEditorWindow::~PhotoEditorWindow()
{
    delete ui;
    delete mainScene;
    delete layer;
}

void PhotoEditorWindow::addPhotoItem(std::unique_ptr<QPhotoItem> &item, QString filePath)
{
    item->setParentItem(layer);
    item->setPos(layer->getSize().width() / 2 - item->getSize().width() / 2,
                 layer->getSize().height() / 2 - item->getSize().height() / 2);

    auto listWidgetItem = std::make_unique<QListWidgetItem>(QIcon(filePath),
                                                            item->getName(),
                                                            ui->listItems);
    listWidgetItem->setData(Qt::UserRole, QVariant(itemsCount));
    listWidgetItems.emplace(itemsCount, std::move(listWidgetItem));

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
        listWidgetItems.at(item.getId())->setSelected(!item.isSelected());
        ui->gbPos->setVisible(!item.isSelected());
        ui->gbSize->setVisible(!item.isSelected());
        ui->gbTranspose->setVisible(!item.isSelected());
        ui->btnUp->setVisible(!item.isSelected());
        ui->gbFilters->setVisible(!item.isSelected());

        if (!item.isSelected()) {
            ui->edtWidth->setText(QString::number(item.getCroppedSize().width()));
            ui->edtHeight->setText(QString::number(item.getCroppedSize().height()));

            ui->edtX->setText(QString::number(item.getPos().x()));
            ui->edtY->setText(QString::number(item.getPos().y()));

            ui->sliderAlpha->setValue(item.getAlpha());

            ui->cbFilters->setCurrentIndex(static_cast<int>(item.getCurrFilter()));

            ui->sliderContrast->setValue(item.getContrastVal());
        }
        break;
    case QPhotoItem::PhotoItemChanged::ItemSizeChanged:
        ui->edtX->setText(QString::number(item.getPos().x()));
        ui->edtY->setText(QString::number(item.getPos().y()));
        ui->edtWidth->setText(QString::number(item.getCroppedSize().width()));
        ui->edtHeight->setText(QString::number(item.getCroppedSize().height()));
        break;
    case QPhotoItem::PhotoItemChanged::ItemPositionChanged:
        ui->edtX->setText(QString::number(item.getPos().x()));
        ui->edtY->setText(QString::number(item.getPos().y()));
        break;
    }
}

void PhotoEditorWindow::updateLayerView(QBaseLayer &layer)
{
    listWidgetItems.at(-1)->setSelected(!layer.isSelected());
    ui->gbSize->setVisible(!layer.isSelected());
    ui->edtWidth->setText(QString::number(layer.getSize().width()));
    ui->edtHeight->setText(QString::number(layer.getSize().height()));
}

void PhotoEditorWindow::keyPressEvent(QKeyEvent *event)
{
    if (ui->listItems->selectedItems().size() > 0
        && ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt() != -1) {
        if (event->key() == Qt::Key_Delete) {
            int id = ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt();
            items.at(id)->setSelected(false);
            items.erase(id);
            listWidgetItems.erase(id);
        }
    }
    if (event->key() == Qt::Key_Plus) {
        ui->mainGraphView->scale(1.05, 1.05);
    } else if (event->key() == Qt::Key_Minus) {
        ui->mainGraphView->scale(1 / 1.05, 1 / 1.05);
    }
    QMainWindow::keyPressEvent(event);
}

void PhotoEditorWindow::on_btnAddItem_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Image",
                                                    QDir::homePath() + "/Pictures/",
                                                    "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        auto item = std::make_unique<QPhotoItem>(filePath, itemsCount);
        addPhotoItem(item, filePath);
    }
}

void PhotoEditorWindow::on_btnSave_clicked()
{
    SaveDialog saveDialog(projectName);
    if (ui->listItems->selectedItems().size() > 0) {
        int id = ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt();
        if (id == -1) {
            layer->setSelected(false);
        } else {
            items.at(id)->setSelected(false);
        }
    }
    if (saveDialog.exec()) {
        QImage image(layer->getSize(), QImage::Format_ARGB32);
        QPainter painter(&image);
        mainScene->render(&painter);
        painter.end();

        if (image.save(saveDialog.getFilePath(),
                       saveDialog.getFormat().toUtf8(),
                       saveDialog.getQuality())) {
            QMessageBox::information(nullptr, "Success", "Your image was successfully saved!");
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to saved the image.");
        }
    }
}

void PhotoEditorWindow::on_listItems_itemClicked(QListWidgetItem *item)
{
    for (auto photoItem : mainScene->selectedItems()) {
        photoItem->setSelected(false);
    }
    int data = item->data(Qt::UserRole).toInt();
    if (data == -1) {
        layer->setSelected(true);
    } else {
        items.at(data)->setSelected(true);
    }
}

void PhotoEditorWindow::on_edtPos_editingFinished()
{
    if (ui->edtX->text().isEmpty()) {
        ui->edtX->setText("0");
    }
    items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())
        ->setCroppedPos(ui->edtX->text().toInt(), ui->edtY->text().toInt());
}

void PhotoEditorWindow::on_edtSize_editingFinished()
{
    int id = ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt();
    if (id == -1) {
        layer->scale(ui->edtWidth->text().toInt(), ui->edtHeight->text().toInt());
    } else {
        items.at(id)->resize(ui->edtWidth->text().toInt(), ui->edtHeight->text().toInt());
    }
}

void PhotoEditorWindow::on_btnRotate_clicked()
{
    items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->rotateClockwise();
}

void PhotoEditorWindow::on_btnFlipH_clicked()
{
    items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->flipH();
}

void PhotoEditorWindow::on_btnFlipV_clicked()
{
    items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->flipV();
}

void PhotoEditorWindow::on_btnUp_clicked()
{
    items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->up();
}

void PhotoEditorWindow::on_sliderAlpha_valueChanged(int value)
{
    ui->edtAlpha->setText(QString::number(value));
    items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->setAlpha(value);
}

void PhotoEditorWindow::on_cbFilters_currentIndexChanged(int index)
{
    if (ui->listItems->selectedItems().size() > 0) {
        switch (ui->cbFilters->currentData().value<QPhotoItem::PhotoFilter>()) {
        case QPhotoItem::PhotoFilter::Null:
            items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->resetFilters();
            break;
        case QPhotoItem::PhotoFilter::Monochrome:
            items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())
                ->applyMonochrome();
            break;
        case QPhotoItem::PhotoFilter::Negative:
            items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->applyNegativ();
            break;
        case QPhotoItem::PhotoFilter::Noise:
            items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->applyNoise();
            break;
        case QPhotoItem::PhotoFilter::Retro:
            items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->applyRetro();
            break;
        case QPhotoItem::PhotoFilter::Sepia:
            items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->applySepia();
            break;
        }
    }
}

void PhotoEditorWindow::on_btnImgToBack_clicked()
{
    auto& item = items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt());
    item->resize(layer->getSize().width(), layer->getSize().height());
    item->setCroppedPos(0, 0);
    updatePhotoView(*item, QPhotoItem::PhotoItemChanged::ItemSizeChanged);
}

void PhotoEditorWindow::on_btnBackToImg_clicked()
{
    auto& item = items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt());
    layer->scale(item->getCroppedSize().width(), item->getCroppedSize().height());
    item->setCroppedPos(0, 0);
    updatePhotoView(*item, QPhotoItem::PhotoItemChanged::ItemPositionChanged);
}

void PhotoEditorWindow::on_sliderContrast_valueChanged(int value)
{
    ui->edtContrast->setText(QString::number(value));
    items.at(ui->listItems->selectedItems()[0]->data(Qt::UserRole).toInt())->applyContrast(value);
}


void PhotoEditorWindow::on_actionCreateFile_triggered()
{
    on_btnSave_clicked();
}

