#ifndef QBASELAYER_H
#define QBASELAYER_H

#include <QGraphicsRectItem>

class QBaseLayer : public QGraphicsRectItem
{
public:
    QBaseLayer(QSize size);
    QBaseLayer(int w, int h);

    void setSize(QSize size);
    void setSize(int w, int h);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QSize layerSize;
};

#endif // QBASELAYER_H
