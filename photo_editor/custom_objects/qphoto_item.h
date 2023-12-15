#ifndef QPHOTOITEM_H
#define QPHOTOITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QListWidget>
#include <QPainter>

class QPhotoItem : public QGraphicsItem
{
    static inline int nexZValue{0};

public:
    QPhotoItem(QString filePath, int id);

    enum class PhotoItemChanged { ItemPositionChanged, ItemSizeChanged, ItemSelectionChanged };

private:
    QString name;
    QImage filteredImage;
    QPixmap drawingPixmap;
    QSizeF croppedSize, sizeBefore, drawingPixmapSize;
    QPointF cursorPosBefore, itemPosBefore;
    qreal top{0}, left{0}, topBefore, leftBefore;
    qreal oldZValue;
    int id;
    int alpha{255};
    int brightness{0};
    std::function<void(QPhotoItem &, PhotoItemChanged)> updateView;

    void paintSelected(QPainter *painter);
    void paintCropping(QPainter *painter);
    void applyChanges();

    enum class State { Disabled = 0, Resizing, Cropping, Moving, Selected };
    State state;

public:
    [[nodiscard]] QSize getSize() const;
    [[nodiscard]] QSize getCroppedSize() const;
    [[nodiscard]] QPoint getPos() const;
    [[nodiscard]] QString getName() const;
    [[nodiscard]] int getId() const;
    [[nodiscard]] int getAlpha() const;
    [[nodiscard]] int getBrightness() const;
    void setBrightness(int newBrightness);
    void setViewUpdate(std::function<void(QPhotoItem &, PhotoItemChanged)> updateView);
    void setCroppedPos(int x, int y);
    void up();
    void rotateClockwise();
    void resize(int width, int height);
    void flipH();
    void flipV();
    void setAlpha(int alpha);
    void monochromeize();

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
};

#endif // QPHOTOITEM_H
