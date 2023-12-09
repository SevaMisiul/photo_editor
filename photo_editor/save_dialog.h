#ifndef SAVE_DIALOG_H
#define SAVE_DIALOG_H

#include <QDialog>

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QString projectName, QWidget *parent = nullptr);
    ~SaveDialog();
    [[nodiscard]] QString getFormat();
    [[nodiscard]] QString getFilePath();
    [[nodiscard]] QString getFullName();
    [[nodiscard]] int getQuality();

private slots:
    void on_btnFilePath_clicked();

    void on_edtPath_textChanged(const QString &arg1);

    void on_cbFormat_currentTextChanged(const QString &arg1);

    void on_edtName_textEdited(const QString &arg1);

    void on_sliderQuality_valueChanged(int value);

private:
    Ui::SaveDialog *ui;
    QString name;

    [[nodiscard]] QString getDirPath();
};

#endif // SAVE_DIALOG_H
