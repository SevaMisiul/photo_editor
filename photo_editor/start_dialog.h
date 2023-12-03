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
    enum class CreateMode { open = 0, create = 1 };

public:
    explicit StartDialog(QWidget *parent = nullptr);
    [[nodiscard]] QSize getSize();
    [[nodiscard]] QString getProjectName();
    [[nodiscard]] QColor getBackColor();
    [[nodiscard]] CreateMode getCreateMode();
    [[nodiscard]] QString getFilePath();
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
    void on_btnCreate_clicked();

private:
    CreateMode createMode;
    Ui::StartDialog *ui;
    int r, g, b;
    QString filePath;
};

#endif // START_DIALOG_H
