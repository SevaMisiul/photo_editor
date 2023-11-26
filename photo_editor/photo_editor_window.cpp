#include "photo_editor_window.h"
#include "./ui_photo_editor_window.h"

#include <QFileDialog>

PhotoEditorWindow::PhotoEditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditorWindow)
{
    ui->setupUi(this);
    mainScene = new QGraphicsScene(this);
    ui->mainGraphView->setScene(mainScene);
}

PhotoEditorWindow::~PhotoEditorWindow()
{
    delete ui;
}

void PhotoEditorWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Image", "", "Images (*.png *.jpg *.bmp)");
    ui->mainGraphView->setMouseTracking(true);
    layer = new QBaseLayer(1000, 1000);
    mainScene->addItem(layer);
    mainScene->setSceneRect(0, 0, 1000, 1000);
    item = new QPhotoItem(QImage(fileName));

    item->setParentItem(layer);
    ui->mainGraphView->setBackgroundBrush(QBrush(Qt::gray));
}

void PhotoEditorWindow::on_pushButton_2_clicked()
{
//    item->rotate(30);
    mainScene->update();
}

