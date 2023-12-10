#include "qbase_layer.h"

#include <QGraphicsScene>
#include <QPainter>

QBaseLayer::QBaseLayer(QSize size, QColor col)
    : layerSize(size)
    , bgColor(col)
    , updateView(nullptr)
{
    setFlag(ItemIsSelectable);
}

void QBaseLayer::scale(int w, int h)
{
    layerSize = QSize(w, h);
    scene()->setSceneRect(0, 0, w, h);
    scene()->update();
}

QSize QBaseLayer::getSize()
{
    return layerSize;
}

void QBaseLayer::setColor(QColor col)
{
    bgColor = col;
}

void QBaseLayer::setViewUpdate(std::function<void(QBaseLayer &)> updateView)
{
    this->updateView = updateView;
}

QRectF QBaseLayer::boundingRect() const
{
    return QRectF(0, 0, layerSize.width(), layerSize.height());
}

void QBaseLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(boundingRect(), bgColor);
    if (isSelected()) {
        painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(-1, -1, 0, 0));
    }
}

QVariant QBaseLayer::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedChange) {
        if (isSelected() != value.toBool() && updateView) {
            updateView(*this);
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void QBaseLayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for (auto item : childItems()) {
        item->setSelected(false);
    }
    QGraphicsItem::mousePressEvent(event);
}
