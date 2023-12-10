#ifndef QBASELAYER_H
#define QBASELAYER_H

#include <QGraphicsItem>

class QBaseLayer : public QGraphicsItem
{
public:
    QBaseLayer(QSize size, QColor col);

    void scale(int w, int h);
    [[nodiscard]] QSize getSize();

    void setColor(QColor col);
    void setViewUpdate(std::function<void(QBaseLayer &)> updateView);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

private:
    QColor bgColor;
    QSize layerSize;
    std::function<void(QBaseLayer &)> updateView;

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // QBASELAYER_H
