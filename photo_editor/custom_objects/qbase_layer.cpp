#include "qbase_layer.h"

#include <QPainter>

QBaseLayer::QBaseLayer(QSize size, QColor col)
    : layerSize(size)
    , bgColor(col)
{
}

QBaseLayer::QBaseLayer(QSize size)
    : layerSize(size)
    , bgColor(255, 255, 255)
{}

QBaseLayer::QBaseLayer(int w, int h)
    : layerSize(w, h)
    , bgColor(255, 255, 255)
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

QSize QBaseLayer::getSize()
{
    return layerSize;
}

void QBaseLayer::setColor(QColor col)
{
    bgColor = col;
}

QRectF QBaseLayer::boundingRect() const
{
    return QRectF(QPoint(0, 0), layerSize);
}

void QBaseLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(boundingRect(), bgColor);
}
