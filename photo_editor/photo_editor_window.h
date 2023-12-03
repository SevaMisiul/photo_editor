#ifndef PHOTOEDITORWINDOW_H
#define PHOTOEDITORWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include "custom_objects/qbase_layer.h"
#include "custom_objects/qphoto_item.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class PhotoEditorWindow;
}
QT_END_NAMESPACE

class PhotoEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    PhotoEditorWindow(QColor bgColor, QSize bgSize, QString name, QWidget *parent = nullptr);
    PhotoEditorWindow(QString filePath, QWidget *parent = nullptr);
    ~PhotoEditorWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_mainGraphView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

private:
    int itemsCount;
    Ui::PhotoEditorWindow *ui;
    QGraphicsScene *mainScene;
    QString projectName;
    std::vector<QPhotoItem *> items;
    QBaseLayer *layer;
};
#endif // PHOTOEDITORWINDOW_H
