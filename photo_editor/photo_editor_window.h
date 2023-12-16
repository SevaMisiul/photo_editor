#ifndef PHOTOEDITORWINDOW_H
#define PHOTOEDITORWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include "custom_objects/qbase_layer.h"
#include "custom_objects/qphoto_item.h"
#include <unordered_map>

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
    ~PhotoEditorWindow();
    void addPhotoItem(std::unique_ptr<QPhotoItem> &item, QString filePath);

private slots:
    void on_btnAddItem_clicked();

    void on_btnSave_clicked();

    void on_listItems_itemClicked(QListWidgetItem *item);

    void on_btnRotate_clicked();

    void on_btnFlipH_clicked();

    void on_btnFlipV_clicked();

    void on_btnUp_clicked();

    void on_sliderAlpha_valueChanged(int value);

    void on_cbFilters_currentIndexChanged(int index);

    void on_btnImgToBack_clicked();

    void on_btnBackToImg_clicked();

    void on_sliderContrast_valueChanged(int value);

    void on_actionCreateFile_triggered();

private:
    int itemsCount;
    Ui::PhotoEditorWindow *ui;
    QGraphicsScene *mainScene;
    QString projectName;
    std::unordered_map<int, std::unique_ptr<QPhotoItem>> items;
    std::unordered_map<int, std::unique_ptr<QListWidgetItem>> listWidgetItems;
    QBaseLayer *layer;

private:
    void updatePhotoView(QPhotoItem &item, QPhotoItem::PhotoItemChanged change);
    void updateLayerView(QBaseLayer &layer);
    void on_edtPos_editingFinished();
    void on_edtSize_editingFinished();

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};
#endif // PHOTOEDITORWINDOW_H
