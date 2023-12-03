#ifndef QPHOTOITEM_H
#define QPHOTOITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

class QPhotoItem : public QGraphicsItem
{
public:
    QPhotoItem(QString filePath, std::vector<QPhotoItem*>* items);

private:
    QString name;
    QImage originalImage;
    QPixmap drawingPixmap;
    QSizeF croppedSize, sizeBefore, drawingPixmapSize;
    QPointF cursorPosBefore, itemPosBefore;
    qreal top, left, topBefore, leftBefore;
    qreal oldZValue;
    std::vector<QPhotoItem*>* items;

    void paintSelected(QPainter *painter);
    void paintCropping(QPainter *painter);

    enum class State { Disabled = 0, Resizing, Cropping, Moving, Selected };
    State state;

public:
    [[nodiscard]] QSize getSize();
    [[nodiscard]] QString getName();
    void rotateClockwise();

protected:
    void resizeOnDrag(QPointF delta);
    void croppOnDrag(QPointF delta);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // QPHOTOITEM_H
