#include "start_dialog.h"
#include "color_validator/qcolor_validator.h"
#include "ui_start_dialog.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QIntValidator>

StartDialog::StartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartDialog)
    , r(255)
    , g(255)
    , b(255)
    , filePath()
{
    ui->setupUi(this);
    QIntValidator *sizeValidator = new QIntValidator(this);
    QColorValidator *colorValidator = new QColorValidator(this);

    ui->editHeight->setValidator(sizeValidator);
    ui->editWidth->setValidator(sizeValidator);

    connect(ui->btnClose, &QPushButton::clicked, this, &StartDialog::close);
    connect(ui->btnCreate, &QPushButton::clicked, this, &StartDialog::accept);

    ui->btnCreate->setDisabled(true);

    ui->editR->setValidator(colorValidator);
    ui->editG->setValidator(colorValidator);
    ui->editB->setValidator(colorValidator);
    ui->editA->setValidator(colorValidator);
    setButtonColor();

    setWindowTitle("New document");
}

QSize StartDialog::getSize()
{
    return QSize(ui->editWidth->text().toInt(), ui->editHeight->text().toInt());
}

QString StartDialog::getProjectName()
{
    return ui->editProjectName->text();
}

QColor StartDialog::getBackColor()
{
    return QColor(r, g, b, ui->editA->text().toInt());
}

StartDialog::CreateMode StartDialog::getCreateMode()
{
    return createMode;
}

QString StartDialog::getFilePath()
{
    return filePath;
}

StartDialog::~StartDialog()
{
    delete ui;
}

void StartDialog::setButtonColor()
{
    QString css = QString("background-color: rgb(%1, %2, %3);").arg(r).arg(g).arg(b);
    ui->btnColor->setStyleSheet(css);
}

void StartDialog::on_editProjectName_textChanged(const QString &arg1)
{
    ui->btnCreate->setDisabled(arg1.isEmpty() || ui->editHeight->text().isEmpty()
                               || ui->editWidth->text().isEmpty());
}

void StartDialog::on_editWidth_textChanged(const QString &arg1)
{
    ui->btnCreate->setDisabled(arg1.isEmpty() || ui->editProjectName->text().isEmpty()
                               || ui->editHeight->text().isEmpty());
}

void StartDialog::on_editHeight_textChanged(const QString &arg1)
{
    ui->btnCreate->setDisabled(arg1.isEmpty() || ui->editWidth->text().isEmpty()
                               || ui->editProjectName->text().isEmpty());
}

void StartDialog::on_btnOpen_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, "Image", QDir::homePath() + "/Pictures/", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!filePath.isEmpty()) {
        createMode = CreateMode::open;
        accept();
    }
}

void StartDialog::on_editR_textEdited(const QString &arg1)
{
    r = arg1.toInt();
    setButtonColor();
}

void StartDialog::on_editG_textEdited(const QString &arg1)
{
    g = arg1.toInt();
    setButtonColor();
}

void StartDialog::on_editB_textEdited(const QString &arg1)
{
    b = arg1.toInt();
    setButtonColor();
}

void StartDialog::on_btnColor_clicked()
{
    QColorDialog dialog(this);
    QColor newColor = dialog.getColor(QColor(r, g, b));
    ui->editR->setText(QString::number(newColor.red()));
    ui->editG->setText(QString::number(newColor.green()));
    ui->editB->setText(QString::number(newColor.blue()));
    r = newColor.red();
    g = newColor.green();
    b = newColor.blue();
    setButtonColor();
}

void StartDialog::on_btn1920_1080_clicked()
{
    acceptTemplate(1920, 1080);
}

void StartDialog::acceptTemplate(int w, int h)
{
    ui->editA->setText("255");
    ui->editR->setText("255");
    ui->editG->setText("255");
    ui->editB->setText("255");
    ui->editProjectName->setText(QString("Project %1 x %2").arg(w).arg(h));
    ui->editWidth->setText(QString::number(w));
    ui->editHeight->setText(QString::number(h));
    accept();
}

void StartDialog::on_btn1080_1080_clicked()
{
    acceptTemplate(1080, 1080);
}

void StartDialog::on_btn1080_720_clicked()
{
    acceptTemplate(1080, 720);
}

void StartDialog::on_btn1080_1920_clicked()
{
    acceptTemplate(1080, 1920);
}

void StartDialog::on_btn1280_720_clicked()
{
    acceptTemplate(1280, 720);
}

void StartDialog::on_btn1280_1280_clicked()
{
    acceptTemplate(1280, 1280);
}

