#include "qphoto_item.h"

#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPointF>

QPhotoItem::QPhotoItem(QString filePath,
                       QListWidget *listWidget,
                       std::unordered_map<int, std::unique_ptr<QPhotoItem>> &items,
                       int id)
    : originalImage(filePath)
    , state(State::Disabled)
    , left(0)
    , top(0)
    , listWidget(listWidget)
    , items(items)
    , id(id)
{
    drawingPixmap = QPixmap::fromImage(originalImage);
    croppedSize = drawingPixmap.size();
    drawingPixmapSize = drawingPixmap.size();
    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);
    setAcceptHoverEvents(true);

    name = "";
    int i = filePath.length() - 1;
    while (filePath[i] != '.' && i > 0) {
        i--;
    }
    for (i--; i >= 0 && filePath[i] != '/'; i--) {
        name = filePath[i] + name;
    }
}

void QPhotoItem::paintSelected(QPainter *painter)
{
    painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
    painter->drawRect(boundingRect().adjusted(-1, -1, 0, 0));

    painter->setPen(Qt::black);

    QRectF topResizeRect = QRect(left + (croppedSize.width() - 5) / 2, top - 2, 4, 4);
    painter->fillRect(topResizeRect, Qt::white);
    painter->drawRect(topResizeRect);

    QRectF bottomResizeRect = QRect(left + (croppedSize.width() - 5) / 2,
                                    top + croppedSize.height() - 3,
                                    4,
                                    4);
    painter->fillRect(bottomResizeRect, Qt::white);
    painter->drawRect(bottomResizeRect);

    QRectF leftResizeRect = QRect(left - 2, top + (croppedSize.height() - 5) / 2, 4, 4);
    painter->fillRect(leftResizeRect, Qt::white);
    painter->drawRect(leftResizeRect);

    QRectF rightResizeRect = QRect(left + croppedSize.width() - 3,
                                   top + (croppedSize.height() - 5) / 2,
                                   4,
                                   4);
    painter->fillRect(rightResizeRect, Qt::white);
    painter->drawRect(rightResizeRect);

    QRectF leftBottomResizeRect = QRect(left - 2, top + croppedSize.height() - 3, 4, 4);
    painter->fillRect(leftBottomResizeRect, Qt::white);
    painter->drawRect(leftBottomResizeRect);

    QRectF leftTopResizeRect = QRect(left - 2, top - 2, 4, 4);
    painter->fillRect(leftTopResizeRect, Qt::white);
    painter->drawRect(leftTopResizeRect);

    QRectF RightTopResizeRect = QRect(left + croppedSize.width() - 3, top - 2, 4, 4);
    painter->fillRect(RightTopResizeRect, Qt::white);
    painter->drawRect(RightTopResizeRect);

    QRectF rightBottomResizeRect = QRect(left + croppedSize.width() - 3,
                                         top + croppedSize.height() - 3,
                                         4,
                                         4);
    painter->fillRect(rightBottomResizeRect, Qt::white);
    painter->drawRect(rightBottomResizeRect);
}

void QPhotoItem::paintCropping(QPainter *painter)
{
    painter->fillRect(0, 0, left, drawingPixmap.height(), QColor(0, 0, 0, 128));
    painter->fillRect(left, 0, drawingPixmap.width() - left, top, QColor(0, 0, 0, 128));
    painter->fillRect(left,
                      top + croppedSize.height(),
                      drawingPixmap.width() - left,
                      drawingPixmap.height() - top - croppedSize.height(),
                      QColor(0, 0, 0, 128));
    painter->fillRect(left + croppedSize.width(),
                      top,
                      drawingPixmap.width() - left - croppedSize.width(),
                      croppedSize.height(),
                      QColor(0, 0, 0, 128));

    painter->setPen(QPen(Qt::black, 1));
    painter->drawRect(boundingRect().adjusted(-1, -1, 0, 0));

    painter->setPen(QPen(Qt::white, 1));
    QRectF topCroppRect = QRect(left + (croppedSize.width() - 20) / 2, top - 1, 20, 5);
    painter->fillRect(topCroppRect, Qt::black);
    painter->drawRect(topCroppRect);

    QRectF bottomCroppRect = QRect(left + (croppedSize.width() - 20) / 2,
                                   top + croppedSize.height() - 5,
                                   20,
                                   5);
    painter->fillRect(bottomCroppRect, Qt::black);
    painter->drawRect(bottomCroppRect);

    QRectF leftCroppRect = QRect(left - 1, top + (croppedSize.height() - 20) / 2, 5, 20);
    painter->fillRect(leftCroppRect, Qt::black);
    painter->drawRect(leftCroppRect);

    QRectF rightCroppRect = QRect(left + croppedSize.width() - 5,
                                  top + (croppedSize.height() - 20) / 2,
                                  5,
                                  20);
    painter->fillRect(rightCroppRect, Qt::black);
    painter->drawRect(rightCroppRect);

    QRectF leftTopTopRect = QRect(left - 1, top - 1, 20, 5);
    QRectF leftTopLeftRect = QRect(left - 1, top - 1, 5, 20);
    painter->drawRect(leftTopTopRect);
    painter->drawRect(leftTopLeftRect);
    painter->fillRect(leftTopTopRect.adjusted(1, 1, 0, 0), Qt::black);
    painter->fillRect(leftTopLeftRect.adjusted(1, 1, 0, 0), Qt::black);

    QRectF leftBottomBottomRect = QRect(left - 1, top + croppedSize.height() - 5, 20, 5);
    QRectF leftBottomLeftRect = QRect(left - 1, top + croppedSize.height() - 20, 5, 20);
    painter->drawRect(leftBottomBottomRect);
    painter->drawRect(leftBottomLeftRect);
    painter->fillRect(leftBottomBottomRect.adjusted(1, 1, 0, 0), Qt::black);
    painter->fillRect(leftBottomLeftRect.adjusted(1, 1, 0, 0), Qt::black);

    QRectF rightBottomBottomRect = QRect(left + croppedSize.width() - 20,
                                         top + croppedSize.height() - 5,
                                         20,
                                         5);
    QRectF rightBottomRightRect = QRect(left + croppedSize.width() - 5,
                                        top + croppedSize.height() - 20,
                                        5,
                                        20);
    painter->drawRect(rightBottomBottomRect);
    painter->drawRect(rightBottomRightRect);
    painter->fillRect(rightBottomBottomRect.adjusted(1, 1, 0, 0), Qt::black);
    painter->fillRect(rightBottomRightRect.adjusted(1, 1, 0, 0), Qt::black);

    QRectF rightTopTopRect = QRect(left + croppedSize.width() - 20, top - 1, 20, 5);
    QRectF rightTopRightRect = QRect(left + croppedSize.width() - 5, top - 1, 5, 20);
    painter->drawRect(rightTopTopRect);
    painter->drawRect(rightTopRightRect);
    painter->fillRect(rightTopTopRect.adjusted(1, 1, 0, 0), Qt::black);
    painter->fillRect(rightTopRightRect.adjusted(1, 1, 0, 0), Qt::black);
}

QSize QPhotoItem::getSize()
{
    return drawingPixmap.size();
}

QString QPhotoItem::getName()
{
    return name;
}

void QPhotoItem::rotateClockwise()
{
    QTransform transform;
    transform.rotate(90);

    int bottom = drawingPixmapSize.height() - top - croppedSize.height();

    moveBy(left + croppedSize.width() / 2 - bottom - croppedSize.height() / 2,
           top + croppedSize.height() / 2 - left - croppedSize.width() / 2);

    int tmp = left;
    left = bottom;
    top = tmp;

    originalImage = originalImage.transformed(transform, Qt::SmoothTransformation);
    drawingPixmap = drawingPixmap.transformed(transform, Qt::SmoothTransformation);
    drawingPixmapSize = drawingPixmap.size();

    tmp = croppedSize.height();
    croppedSize.setHeight(croppedSize.width());
    croppedSize.setWidth(tmp);

    scene()->update();
}

void QPhotoItem::resizeOnDrag(QPointF cursorPos)
{
    QPoint newPos(0, 0);
    QSizeF newSize(croppedSize);
    qreal newLeft, newTop;
    qreal coeffW, coeffH;
    if (cursorPosBefore.x() < 10 + leftBefore) {
        newSize.setWidth(qMax(15.0, croppedSize.width() - (cursorPos.x() - left)));
        newPos.setX((int) croppedSize.width() - (int) newSize.width());
    } else if (cursorPosBefore.x() > leftBefore + sizeBefore.width() - 10) {
        newSize.setWidth(qMax(15.0, cursorPos.x() - left));
    }
    coeffW = newSize.width() / croppedSize.width();
    drawingPixmapSize.setWidth(drawingPixmapSize.width() * coeffW);
    newLeft = left * coeffW;
    newPos.setX(newPos.x() + (int) left - (int) newLeft);

    if (cursorPosBefore.y() < 10 + topBefore) {
        newSize.setHeight(qMax(15.0, croppedSize.height() - (cursorPos.y() - top)));
        newPos.setY((int) croppedSize.height() - (int) newSize.height());
    } else if (cursorPosBefore.y() > topBefore + sizeBefore.height() - 10) {
        newSize.setHeight(qMax(15.0, cursorPos.y() - top));
    }
    coeffH = newSize.height() / croppedSize.height();
    drawingPixmapSize.setHeight(drawingPixmapSize.height() * coeffH);
    newTop = top * coeffH;
    newPos.setY(newPos.y() + (int) top - (int) newTop);

    moveBy(newPos.x(), newPos.y());

    QTransform transform;
    qreal transformX = drawingPixmapSize.width() / originalImage.width();
    qreal transformY = drawingPixmapSize.height() / originalImage.height();
    transform.scale(transformX, transformY);
    drawingPixmap = QPixmap::fromImage(originalImage)
                        .transformed(transform, Qt::SmoothTransformation);

    left = newLeft;
    top = newTop;
    croppedSize = newSize;
    scene()->update();
}

void QPhotoItem::croppOnDrag(QPointF cursorPos)
{
    int newLeft = left, newTop = top, newWidth = croppedSize.width(),
        newHeight = croppedSize.height();
    qreal right = drawingPixmap.width() - croppedSize.width() - left;
    qreal bottom = drawingPixmap.height() - croppedSize.height() - top;
    if (cursor().shape() == Qt::CursorShape::SizeAllCursor)
        return;
    if (cursorPosBefore.x() < leftBefore + 10) {
        newWidth = qMax(15.0,
                        qMin(drawingPixmap.width() - right,
                             croppedSize.width() + (left - cursorPos.x())));
        newLeft -= newWidth - croppedSize.width();
    } else if (cursorPosBefore.x() > left + sizeBefore.width() - 10) {
        newWidth = qMax(15.0, qMin(drawingPixmap.width() - left, ((int) cursorPos.x() - left)));
    }
    if (cursorPosBefore.y() < topBefore + 10) {
        newHeight = qMax(15.0,
                         qMin(drawingPixmap.height() - bottom,
                              croppedSize.height() + (top - (int) cursorPos.y())));
        newTop -= newHeight - croppedSize.height();
    } else if (cursorPosBefore.y() > top + sizeBefore.height() - 10) {
        newHeight = qMax(15.0, qMin(drawingPixmap.height() - top, (int) cursorPos.y() - top));
    }
    croppedSize.setWidth(newWidth);
    left = newLeft;
    croppedSize.setHeight(newHeight);
    top = newTop;
    scene()->update();
}

QRectF QPhotoItem::boundingRect() const
{
    return QRectF((int) left, (int) top, (int) croppedSize.width(), (int) croppedSize.height());
}

void QPhotoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (state == State::Cropping) {
        painter->drawPixmap(0, 0, drawingPixmap);
        paintCropping(painter);
    } else {
        painter->drawPixmap(QPoint(left, top), drawingPixmap, boundingRect());
        if (state == State::Selected || state == State::Resizing) {
            paintSelected(painter);
        }
    }
}

void QPhotoItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return;
    if (state == State::Selected) {
        setCursor(QCursor(Qt::CursorShape::SizeAllCursor));
        setFlag(ItemIsMovable);
        state = State::Moving;
    } else if (state == State::Resizing || state == State::Cropping) {
        cursorPosBefore = event->pos();
        sizeBefore = croppedSize;
        itemPosBefore = pos();
        leftBefore = left;
        topBefore = top;
        setFlag(ItemIsMovable, false);
    }
    QGraphicsItem::mousePressEvent(event);
}

void QPhotoItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch (state) {
    case State::Resizing:
        resizeOnDrag(event->pos());
        break;
    case State::Cropping:
        croppOnDrag(event->pos());
        break;
    default:
        break;
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void QPhotoItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (state != State::Cropping) {
        state = State::Selected;
        update();
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

void QPhotoItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    state = State::Cropping;
    scene()->update();

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
        if (state != State::Cropping) {
            state = State::Resizing;
        }
        if (mousePos.y() > (top + croppedSize.height() / 2 - 8)
            && mousePos.y() < (top + croppedSize.height() / 2 + 8)
            && (mousePos.x() < left + 10 || mousePos.x() > left + croppedSize.width() - 10)) {
            currCursor = QCursor(Qt::CursorShape::SizeHorCursor);
        } else if (mousePos.x() > (left + croppedSize.width() / 2 - 8)
                   && mousePos.x() < (left + croppedSize.width() / 2 + 8)
                   && (mousePos.y() < top + 10 || mousePos.y() > top + croppedSize.height() - 10)) {
            currCursor = QCursor(Qt::CursorShape::SizeVerCursor);
        } else if (mousePos.x() < left + 10 && mousePos.y() < top + 10
                   || mousePos.x() > left + croppedSize.width() - 10
                          && mousePos.y() > top + croppedSize.height() - 10) {
            currCursor = QCursor(Qt::CursorShape::SizeFDiagCursor);
        } else if (mousePos.x() > left + croppedSize.width() - 10 && mousePos.y() < top + 10
                   || mousePos.x() < left + 10 && mousePos.y() > top + croppedSize.height() - 10) {
            currCursor = QCursor(Qt::CursorShape::SizeBDiagCursor);
        } else {
            currCursor = QCursor(Qt::CursorShape::SizeAllCursor);
            if (state != State::Cropping) {
                state = State::Selected;
            }
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
            scene()->update();
            state = State::Disabled;
            setFlag(ItemIsMovable, false);
            setZValue(oldZValue);
        }
        for (int i = 0; i < listWidget->count(); i++) {
            if (listWidget->item(i)->data(Qt::UserRole).toInt() == id)
                listWidget->item(i)->setSelected(isSelected());
        }
    }

    return QGraphicsItem::itemChange(change, value);
}
