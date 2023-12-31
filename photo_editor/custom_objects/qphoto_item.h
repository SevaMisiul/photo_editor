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

    enum class PhotoFilter { Null = 0, Monochrome = 1, Sepia = 2, Negative = 3, Retro = 4, Noise = 5, Blur = 6};

private:
    QString name;
    QImage filteredImage, originalImage;
    QPixmap drawingPixmap;
    QSizeF croppedSize, sizeBefore, drawingPixmapSize;
    QPointF cursorPosBefore, itemPosBefore;
    qreal top{0}, left{0}, topBefore, leftBefore;
    qreal oldZValue;
    int id;
    int alpha{255};
    int brightness{0};
    std::function<void(QPhotoItem &, PhotoItemChanged)> updateView;
    PhotoFilter currFilter{0};
    int contrastVal{0};

    void paintSelected(QPainter *painter);
    void paintCropping(QPainter *painter);
    void applyChanges();
    void setMonochrome();
    void setSepia();
    void setRetro();
    void setNegative();
    void setContrast(int newVal);
    void setNoise();
    void setBlur();

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
    [[nodiscard]] PhotoFilter getCurrFilter() const;
    [[nodiscard]] int getContrastVal() const;

    void setViewUpdate(std::function<void(QPhotoItem &, PhotoItemChanged)> updateView);
    void setCroppedPos(int x, int y);
    void up();
    void rotateClockwise();
    void resize(int width, int height);
    void flipH();
    void flipV();
    void setAlpha(int alpha);
    void applyMonochrome();
    void applySepia();
    void applyNegativ();
    void applyRetro();
    void applyNoise();
    void applyContrast(int newVal);
    void applyBlur();
    void resetFilters();

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

Q_DECLARE_METATYPE(QPhotoItem::PhotoFilter)

#endif // QPHOTOITEM_H
