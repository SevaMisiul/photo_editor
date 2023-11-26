#ifndef START_DIALOG_H
#define START_DIALOG_H

#include <QDialog>

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    [[nodiscard]] QSize getSize();
    [[nodiscard]] QString getProjectName();
    [[nodiscard]] QColor getBackColor();
    ~StartDialog();

protected:
    void setButtonColor();

private slots:
    void on_editProjectName_textChanged(const QString &arg1);
    void on_editWidth_textChanged(const QString &arg1);
    void on_editHeight_textChanged(const QString &arg1);
    void on_btnOpen_clicked();
    void on_editR_textEdited(const QString &arg1);
    void on_editG_textEdited(const QString &arg1);
    void on_editB_textEdited(const QString &arg1);
    void on_btnColor_clicked();

private:
    Ui::StartDialog *ui;
    int r, g, b;
};

#endif // START_DIALOG_H
