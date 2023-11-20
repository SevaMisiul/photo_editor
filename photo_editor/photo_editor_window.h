#ifndef PHOTOEDITORWINDOW_H
#define PHOTOEDITORWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PhotoEditorWindow; }
QT_END_NAMESPACE

class PhotoEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    PhotoEditorWindow(QWidget *parent = nullptr);
    ~PhotoEditorWindow();

private:
    Ui::PhotoEditorWindow *ui;
};
#endif // PHOTOEDITORWINDOW_H
