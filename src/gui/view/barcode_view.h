#ifndef GUI_VIEW_BARCODE_VIEW_H_
#define GUI_VIEW_BARCODE_VIEW_H_

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

signals:
    void ZoomLevelChanged(double zoom_level, const QSize &image_size);

public slots:
    void FitToScreen();

private slots:
    void UpdateHighQualityPixmap();

private:
    QGraphicsScene *scene_graphics_;
    QGraphicsPixmapItem *pixmap_item_;

    QPixmap pixmap_src_;           // Original high-res image
    double pixmap_scale_factor_;   // Tracks current zoom level
    double initial_scale_factor_;  // Initial zoom level when image is first loaded
    QTimer *timer_zoom_;           // Debounce timer for high-quality render;
};

#endif