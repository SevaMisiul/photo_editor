#include "photo_editor_window.h"
#include "./ui_photo_editor_window.h"

PhotoEditorWindow::PhotoEditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoEditorWindow)
{
    ui->setupUi(this);
}

PhotoEditorWindow::~PhotoEditorWindow()
{
    delete ui;
}

