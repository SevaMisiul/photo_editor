#ifndef QBASELAYER_H
#define QBASELAYER_H

#include <QGraphicsRectItem>

class QBaseLayer : public QGraphicsRectItem
{
public:
    QBaseLayer(QSize size, QColor col);
    QBaseLayer(QSize size);
    QBaseLayer(int w, int h);

    void setSize(QSize size);
    void setSize(int w, int h);
    [[nodiscard]] QSize getSize();

    void setColor(QColor col);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

private:
    QColor bgColor;
    QSize layerSize;
};

#endif // QBASELAYER_H
