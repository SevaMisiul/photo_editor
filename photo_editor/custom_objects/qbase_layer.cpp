#include "qbase_layer.h"

#include <QPainter>

QBaseLayer::QBaseLayer(QSize size)
    : layerSize(size)
{}

QBaseLayer::QBaseLayer(int w, int h)
    : layerSize(w, h)
{}

void QBaseLayer::setSize(QSize size)
{
    layerSize = size;
    update();
}

void QBaseLayer::setSize(int w, int h)
{
    layerSize = QSize(w, h);
    update();
}

QRectF QBaseLayer::boundingRect() const
{
    return QRectF(QPoint(0, 0), layerSize);
}

void QBaseLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(boundingRect(), Qt::white);
}
