#ifndef QCUSTOMLISTWIDGET_H
#define QCUSTOMLISTWIDGET_H

#include <QListWidget>
#include <QKeyEvent>

class QCustomListWidget : public QListWidget
{
    Q_OBJECT
public:
    QCustomListWidget(QWidget* parent);

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

inline QCustomListWidget::QCustomListWidget(QWidget *parent):
    QListWidget(parent)
{

}

inline void QCustomListWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        return;
    }
    QListWidget::keyPressEvent(event);
}

#endif // QCUSTOMLISTWIDGET_H
