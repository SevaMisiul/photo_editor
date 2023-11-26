#include "photo_editor_window.h"
#include "./ui_photo_editor_window.h"

#include <QFileDialog>

PhotoEditorWindow::PhotoEditorWindow(QColor bgColor, QSize bgSize, QString name, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditorWindow)
    , projectName(name)
{
    ui->setupUi(this);
    mainScene = new QGraphicsScene(this);
    ui->mainGraphView->setScene(mainScene);
    ui->mainGraphView->setMouseTracking(true);
    layer = new QBaseLayer(bgSize, bgColor);
    mainScene->addItem(layer);
    mainScene->setSceneRect(0, 0, bgSize.width(), bgSize.height());
    ui->mainGraphView->setBackgroundBrush(QBrush(Qt::gray));
}

PhotoEditorWindow::~PhotoEditorWindow()
{
    delete ui;
    delete mainScene;
    delete layer;
}

void PhotoEditorWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Image", "", "Images (*.png *.jpg *.bmp)");
    item = new QPhotoItem(QImage(fileName));
    item->setParentItem(layer);
}

void PhotoEditorWindow::on_pushButton_2_clicked()
{
    item->rotate(30);
    mainScene->update();
}
