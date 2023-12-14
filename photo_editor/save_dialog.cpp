#include "save_dialog.h"
#include "ui_save_dialog.h"

#include <QFileDialog>

SaveDialog::SaveDialog(QString projectName, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SaveDialog)
    , name(projectName)
{
    ui->setupUi(this);
    ui->edtName->setText(QString("%1.%2").arg(projectName, getFormat()));

    connect(ui->btnCancel, &QPushButton::clicked, this, &SaveDialog::close);
    connect(ui->btnSave, &QPushButton::clicked, this, &SaveDialog::accept);

    ui->edtPath->setText(QDir::homePath() + "/Pictures/" + ui->edtName->text());

    setWindowTitle("Save");
}

SaveDialog::~SaveDialog()
{
    delete ui;
}

QString SaveDialog::getFormat()
{
    return ui->cbFormat->currentText().toLower();
}

QString SaveDialog::getFilePath()
{
    return ui->edtPath->text();
}

QString SaveDialog::getFullName()
{
    return name + '.' + getFormat();
}

int SaveDialog::getQuality()
{
    return ui->sliderQuality->value();
}

void SaveDialog::on_btnFilePath_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Save Image",
                                                    QDir::homePath() + "/Pictures/"
                                                        + ui->edtName->text(),
                                                    QString("Image (*.%1)").arg(getFormat()));
    if (!filePath.isEmpty()) {
        ui->edtPath->setText(filePath);
    }
}

void SaveDialog::on_edtPath_textChanged(const QString &arg1)
{
    ui->btnSave->setEnabled(!arg1.isEmpty());
}

void SaveDialog::on_cbFormat_currentTextChanged(const QString &arg1)
{
    ui->edtName->setText(getFullName());
    ui->edtPath->setText(getDirPath() + getFullName());
}

void SaveDialog::on_edtName_textEdited(const QString &arg1)
{
    name = arg1.left(arg1.lastIndexOf('.'));
    ui->edtPath->setText(getDirPath() + getFullName());
}

QString SaveDialog::getDirPath()
{
    int id = getFilePath().lastIndexOf('/');
    if (id == -1)
        return QString();
    return getFilePath().left(id) + '/';
}

void SaveDialog::on_sliderQuality_valueChanged(int value)
{
    ui->edtQuality->setText(QString::number(value));
}

