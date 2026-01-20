#pragma once

#include <QGraphicsView>

class QGraphicsScene;
class QGraphicsPixmapItem;
class QTimer;
class QPixmap;

class BarcodeView : public QGraphicsView {
    Q_OBJECT

public:
    explicit BarcodeView(QWidget *parent = nullptr);
    void DisplayImage(const QPixmap &pixmap);

protected:
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void UpdateHighQualityPixmap();

private:
    QGraphicsScene *scene_;
    QGraphicsPixmapItem *pixmap_item_;

    QPixmap pixmap_src_;          // Original high-res image
    double pixmap_scale_factor_;  // Tracks current zoom level
    QTimer *timer_zoom_;          // Debounce timer for high-quality render
};