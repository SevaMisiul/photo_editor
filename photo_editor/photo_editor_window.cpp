#include "photo_editor_window.h"
#include "./ui_photo_editor_window.h"

#include <QFileDialog>
#include <QString>

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

    std::unique_ptr<QPhotoItem> item{std::make_unique<QPhotoItem>(filePath, ui->listWidget, items, itemsCount)};

    mainScene = new QGraphicsScene(this);
    mainScene->setSceneRect(0, 0, item->getSize().width(), item->getSize().height());

    ui->mainGraphView->setScene(mainScene);
    ui->mainGraphView->setMouseTracking(true);
    ui->mainGraphView->setBackgroundBrush(QBrush(Qt::gray));

    layer = new QBaseLayer(item->getSize(), QColor(255, 255, 255));
    mainScene->addItem(layer);

    setWindowTitle(item->getName() + " - Photo editor");
    addItem(item, filePath);
}

PhotoEditorWindow::~PhotoEditorWindow()
{
    delete ui;
    delete mainScene;
    delete layer;
}

void PhotoEditorWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Image", "", "Images (*.png *.jpg *.bmp)");
    auto item = std::make_unique<QPhotoItem>(filePath, ui->listWidget, items, itemsCount);
    addItem(item, filePath);
}

void PhotoEditorWindow::on_pushButton_2_clicked() {}

void PhotoEditorWindow::addItem(std::unique_ptr<QPhotoItem>& item, QString filePath)
{
    item->setParentItem(layer);
    item->setPos(layer->getSize().width() / 2 - item->getSize().width() / 2,
                 layer->getSize().height() / 2 - item->getSize().height() / 2);

    QListWidgetItem *listItem = new QListWidgetItem(item->getName(), ui->listWidget);
    listItem->setIcon(QIcon(filePath));
    listItem->setData(Qt::UserRole, QVariant(itemsCount));
    items.insert({itemsCount++, std::move(item)});
}

