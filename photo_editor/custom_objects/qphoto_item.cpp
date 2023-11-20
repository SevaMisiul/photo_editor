#include "qphoto_item.h"

#include <QCursor>
#include <QGraphicsSceneMouseEvent>

QPhotoItem::QPhotoItem(QImage img)
    : originalImage(img)
    , state(State::Disabled)
{
    drawingPixmap = QPixmap::fromImage(originalImage);
    pixmapSize = drawingPixmap.size();
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

void QPhotoItem::resizeOnDrag(QPointF delta)
{
    QPoint newPos(0, 0);
    QSize newSize(pixmapSize);
    if (cursorPosBefore.x() < 10) {
        newSize.setWidth(pixmapSize.width() - delta.x());
        newPos.setX(delta.x());
    } else if (cursorPosBefore.x() > sizeBefore.width() - 10) {
        newSize.setWidth(delta.x());
    }
    if (cursorPosBefore.y() < 10) {
        newSize.setHeight(pixmapSize.height() - delta.y());
        newPos.setY(delta.y());
    } else if (cursorPosBefore.y() > sizeBefore.height() - 10) {
        newSize.setHeight(delta.y());
    }
    if (newSize.height() > 15 && newSize.width() > 15) {
        moveBy(newPos.x(), newPos.y());
        drawingPixmap = QPixmap::fromImage(originalImage).scaled(newSize);
        pixmapSize = drawingPixmap.size();
    }
    scene()->update();
}

QRectF QPhotoItem::boundingRect() const
{
    return QRectF(0, 0, pixmapSize.width(), pixmapSize.height());
}

void QPhotoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, drawingPixmap);
    if (state == State::Selected || state == State::Resizing) {
        painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(-1, -1, 0, 0));

        QRectF topResizeRect = boundingRect().adjusted(pixmapSize.width() / 2 - 2,
                                                       -2,
                                                       -pixmapSize.width() / 2 + 2,
                                                       -pixmapSize.height() + 2);
        painter->fillRect(topResizeRect, Qt::white);
        painter->setPen(Qt::black);
        painter->drawRect(topResizeRect);

        QRectF bottomResizeRect = boundingRect().adjusted(pixmapSize.width() / 2 - 2,
                                                          pixmapSize.height() - 3,
                                                          -pixmapSize.width() / 2 + 2,
                                                          1);
        painter->fillRect(bottomResizeRect, Qt::white);
        painter->drawRect(bottomResizeRect);

        QRectF leftResizeRect = boundingRect().adjusted(-2,
                                                        pixmapSize.height() / 2 - 2,
                                                        -pixmapSize.width() + 2,
                                                        -pixmapSize.height() / 2 + 2);
        painter->fillRect(leftResizeRect, Qt::white);
        painter->drawRect(leftResizeRect);

        QRectF rightResizeRect = boundingRect().adjusted(pixmapSize.width() - 3,
                                                         pixmapSize.height() / 2 - 2,
                                                         1,
                                                         -pixmapSize.height() / 2 + 2);
        painter->fillRect(rightResizeRect, Qt::white);
        painter->drawRect(rightResizeRect);

        QRectF leftBottomResizeRect = boundingRect().adjusted(-2,
                                                              pixmapSize.height() - 3,
                                                              -pixmapSize.width() + 2,
                                                              1);
        painter->fillRect(leftBottomResizeRect, Qt::white);
        painter->drawRect(leftBottomResizeRect);

        QRectF leftTopResizeRect = boundingRect().adjusted(-2,
                                                           -2,
                                                           -pixmapSize.width() + 2,
                                                           -pixmapSize.height() + 2);
        painter->fillRect(leftTopResizeRect, Qt::white);
        painter->drawRect(leftTopResizeRect);

        QRectF RightTopResizeRect = boundingRect().adjusted(pixmapSize.width() - 3,
                                                            -2,
                                                            1,
                                                            -pixmapSize.height() + 2);
        painter->fillRect(RightTopResizeRect, Qt::white);
        painter->drawRect(RightTopResizeRect);

        QRectF rightBottomResizeRect = boundingRect().adjusted(pixmapSize.width() - 3,
                                                               pixmapSize.height() - 3,
                                                               1,
                                                               1);
        painter->fillRect(rightBottomResizeRect, Qt::white);
        painter->drawRect(rightBottomResizeRect);
    }
}

void QPhotoItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && state == State::Selected) {
        setCursor(QCursor(Qt::CursorShape::SizeAllCursor));
        setFlag(ItemIsMovable);
        state = State::Moving;
    } else if (state == State::Resizing) {
        cursorPosBefore = event->pos();
        sizeBefore = pixmapSize;
        itemPosBefore = pos();
        setFlag(ItemIsMovable, false);
    }
    if (state == State::Resizing || state == State::Moving) {
        scene()->setSceneRect(scene()->sceneRect());
    }
    QGraphicsItem::mousePressEvent(event);
}

void QPhotoItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (state == State::Resizing) {
        resizeOnDrag(event->pos());
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void QPhotoItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (state == State::Resizing || state == State::Moving) {
        qDebug() << "before" << scene()->sceneRect();
        scene()->setSceneRect(QRectF());
        scene()->update();
        qDebug() << "after" << scene()->sceneRect();
    }
    if (state != State::Cropping) {
        state = State::Selected;
        update();
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

void QPhotoItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void QPhotoItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (state == State::Selected)
        setCursor(QCursor(Qt::CursorShape::SizeAllCursor));

    QGraphicsItem::hoverEnterEvent(event);
}

void QPhotoItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (isSelected()) {
        QPointF mousePos = event->pos();
        QCursor currCursor;
        state = State::Resizing;
        if (mousePos.y() > (pixmapSize.height() / 2 - 8)
            && mousePos.y() < (pixmapSize.height() / 2 + 8)
            && (mousePos.x() < 10 || mousePos.x() > pixmapSize.width() - 10)) {
            currCursor = QCursor(Qt::CursorShape::SizeHorCursor);
        } else if (mousePos.x() > (pixmapSize.width() / 2 - 8)
                   && mousePos.x() < (pixmapSize.width() / 2 + 8)
                   && (mousePos.y() < 10 || mousePos.y() > pixmapSize.height() - 10)) {
            currCursor = QCursor(Qt::CursorShape::SizeVerCursor);
        } else if (mousePos.x() < 10 && mousePos.y() < 10
                   || mousePos.x() > pixmapSize.width() - 10
                          && mousePos.y() > pixmapSize.height() - 10) {
            currCursor = QCursor(Qt::CursorShape::SizeFDiagCursor);
        } else if (mousePos.x() > pixmapSize.width() - 10 && mousePos.y() < 10
                   || mousePos.x() < 10 && mousePos.y() > pixmapSize.height() - 10) {
            currCursor = QCursor(Qt::CursorShape::SizeBDiagCursor);
        } else {
            currCursor = QCursor(Qt::CursorShape::SizeAllCursor);
            state = State::Selected;
        }
        setCursor(currCursor);
    }

    QGraphicsItem::hoverMoveEvent(event);
}

void QPhotoItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    unsetCursor();

    QGraphicsItem::hoverLeaveEvent(event);
}

QVariant QPhotoItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        if (isSelected() == true) {
            state = State::Selected;
            oldZValue = zValue();
            setZValue(std::numeric_limits<double>::max());
        } else {
            state = State::Disabled;
            setFlag(ItemIsMovable, false);
            setZValue(oldZValue);
        }
    }

    return QGraphicsItem::itemChange(change, value);
}
