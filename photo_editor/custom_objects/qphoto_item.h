#ifndef QPHOTOITEM_H
#define QPHOTOITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

class QPhotoItem : public QGraphicsItem
{
public:
    QPhotoItem(QImage &&img);

private:
    int angle;
    QImage originalImage;
    QPixmap drawingPixmap;
    QSizeF croppedSize, sizeBefore, drawingPixmapSize;
    QPointF cursorPosBefore, itemPosBefore;
    qreal top, left, topBefore, leftBefore;
    qreal oldZValue;

    void paintSelected(QPainter *painter);
    void paintCropping(QPainter *painter);

    enum class State { Disabled = 0, Resizing, Cropping, Moving, Selected };
    State state;

protected:
    void resizeOnDrag(QPointF delta);
    void croppOnDrag(QPointF delta);

    // QGraphicsItem interface
public:
    void rotate(qreal degree);
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
};

#endif // QPHOTOITEM_H
