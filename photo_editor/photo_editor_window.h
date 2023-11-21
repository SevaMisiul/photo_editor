#ifndef PHOTOEDITORWINDOW_H
#define PHOTOEDITORWINDOW_H

#include "custom_objects/qbase_layer.h"
#include "custom_objects/qphoto_item.h"
#include <QGraphicsScene>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class PhotoEditorWindow;
}
QT_END_NAMESPACE

class PhotoEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    PhotoEditorWindow(QWidget *parent = nullptr);
    ~PhotoEditorWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::PhotoEditorWindow *ui;
    QGraphicsScene *mainScene;
    QPhotoItem* item;
    QBaseLayer* layer;
};
#endif // PHOTOEDITORWINDOW_H
