#include "start_dialog.h"
#include "color_validator/qcolor_validator.h"
#include "ui_start_dialog.h"

#include <QColorDialog>
#include <QIntValidator>

StartDialog::StartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartDialog)
    , r(255)
    , g(255)
    , b(255)
{
    ui->setupUi(this);
    QIntValidator *sizeValidator = new QIntValidator(this);
    QColorValidator *colorValidator = new QColorValidator(this);

    ui->editHeight->setValidator(sizeValidator);
    ui->editWidth->setValidator(sizeValidator);

    connect(ui->btnCreate, &QPushButton::clicked, this, &StartDialog::accept);
    connect(ui->btnClose, &QPushButton::clicked, this, &StartDialog::close);

    ui->btnCreate->setDisabled(true);

    ui->editR->setValidator(colorValidator);
    ui->editG->setValidator(colorValidator);
    ui->editB->setValidator(colorValidator);
    setButtonColor();
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
    return QColor(r, g, b);
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

void StartDialog::on_btnOpen_clicked() {}

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
