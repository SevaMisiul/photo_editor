#include "photo_editor_window.h"
#include "./ui_photo_editor_window.h"

#include <QFileDialog>
#include <QString>

PhotoEditorWindow::PhotoEditorWindow(QColor bgColor, QSize bgSize, QString name, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditorWindow)
    , projectName(name)
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

    items = std::vector<QPhotoItem *>(10);
}

PhotoEditorWindow::PhotoEditorWindow(QString filePath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditorWindow)
{
    ui->setupUi(this);

    items = std::vector<QPhotoItem *>(10);
    QPhotoItem *item = new QPhotoItem(filePath, &items);
    items.push_back(item);

    mainScene = new QGraphicsScene(this);
    mainScene->setSceneRect(0, 0, item->getSize().width(), item->getSize().height());

    ui->mainGraphView->setScene(mainScene);
    ui->mainGraphView->setMouseTracking(true);
    ui->mainGraphView->setBackgroundBrush(QBrush(Qt::gray));

    layer = new QBaseLayer(item->getSize(), QColor(255, 255, 255));
    mainScene->addItem(layer);
    item->setParentItem(layer);
    item->setPos(layer->getSize().width() / 2 - item->getSize().width() / 2,
                 layer->getSize().height() / 2 - item->getSize().height() / 2);

    setWindowTitle(item->getName() + " - Photo editor");
}

PhotoEditorWindow::~PhotoEditorWindow()
{
    delete ui;
    delete mainScene;
    delete layer;
    for (auto el : items) {
        delete el;
    }
}

void PhotoEditorWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Image", "", "Images (*.png *.jpg *.bmp)");
    QPhotoItem *item = new QPhotoItem(filePath, &items);
    items.push_back(item);
    item->setParentItem(layer);
    item->setPos(layer->getSize().width() / 2 - item->getSize().width() / 2,
                 layer->getSize().height() / 2 - item->getSize().height() / 2);

    QListWidgetItem *listItem = new QListWidgetItem(item->getName());
    listItem->setIcon(QIcon(filePath));
    ui->listWidget->addItem(listItem);
}

void PhotoEditorWindow::on_pushButton_2_clicked() {}
