#include "barcode_view.h"

#include "utils/constants.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QOpenGLWidget>
#include <QPixmap>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>

BarcodeView::BarcodeView(QWidget *parent)
    : QGraphicsView(parent),
      scene_(new QGraphicsScene(this)),
      pixmap_item_(nullptr),
      pixmap_scale_factor_(DisplayConfig::kDefaultScale) {
    setViewport(new QOpenGLWidget());
    setScene(scene_);
    // UI Setup
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setBackgroundBrush(DisplayConfig::kBackgroundCanvasColor);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Disable built-in anchors to handle manual high-quality re-centering math
    setTransformationAnchor(QGraphicsView::NoAnchor);
    // Setup debounce timer
    timer_zoom_ = new QTimer(this);
    timer_zoom_->setSingleShot(true);
    connect(timer_zoom_, &QTimer::timeout, this, &BarcodeView::UpdateHighQualityPixmap);
}

void BarcodeView::DisplayImage(const QPixmap &pixmap) {
    if (pixmap.isNull()) return;

    pixmap_src_ = pixmap;
    pixmap_scale_factor_ = DisplayConfig::kDefaultScale;

    // Fit to view initially if possible
    if (!pixmap_src_.isNull() && viewport()->width() > 0) {
        double view_w = viewport()->width();
        double view_h = viewport()->height();
        pixmap_scale_factor_ = qMin(view_w / pixmap_src_.width(), view_h / pixmap_src_.height());
    }

    UpdateHighQualityPixmap();
}

void BarcodeView::wheelEvent(QWheelEvent *event) {
    if (pixmap_src_.isNull()) return;

    // Capture current mouse position in scene to keep it anchored
    const QPointF view_pt = event->position();
    const QPointF scene_pt_before = mapToScene(view_pt.toPoint());

    // Fast GPU Zoom (Stretches the current pixmap item texture)
    if (event->angleDelta().y() > 0) {
        scale(DisplayConfig::kZoomStep, DisplayConfig::kZoomStep);
        pixmap_scale_factor_ *= DisplayConfig::kZoomStep;
    } else {
        if (transform().m11() > DisplayConfig::kMinScaleLimit) {
            scale(1.0 / DisplayConfig::kZoomStep, 1.0 / DisplayConfig::kZoomStep);
            pixmap_scale_factor_ /= DisplayConfig::kZoomStep;
        }
    }

    // Manual anchor math, adjust scrollbars to keep point under mouse stationary
    const QPointF scene_pt_after = mapToScene(view_pt.toPoint());
    const QPointF delta = scene_pt_after - scene_pt_before;
    // Apply delta weighted by the current transformation matrix
    horizontalScrollBar()->setValue(
        qRound(horizontalScrollBar()->value() - delta.x() * transform().m11())
    );
    verticalScrollBar()->setValue(
        qRound(verticalScrollBar()->value() - delta.y() * transform().m22())
    );
    // Restart debounce timer for the high-quality CPU redraw
    timer_zoom_->start(DisplayConfig::kZoomDebounceMs);
    event->accept();
}

void BarcodeView::UpdateHighQualityPixmap() {
    if (pixmap_src_.isNull()) return;

    // Capture relative look-at point to maintain view focus after resample
    const double scene_w = scene_->width();
    const double scene_h = scene_->height();
    double rel_x = DisplayConfig::kDefaultRelativePos;
    double rel_y = DisplayConfig::kDefaultRelativePos;

    if (scene_w > 0 && scene_h > 0) {
        QPointF currentCenter = mapToScene(viewport()->rect().center());
        rel_x = currentCenter.x() / scene_w;
        rel_y = currentCenter.y() / scene_h;
    }

    // Validate scale factor
    if (std::isnan(pixmap_scale_factor_) || pixmap_scale_factor_ <= 0) {
        pixmap_scale_factor_ = DisplayConfig::kFallbackScale;
    }

    // High-Quality CPU Resample
    const QSize new_size = pixmap_src_.size() * pixmap_scale_factor_;
    QPixmap high_qual = pixmap_src_.scaled(new_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (!pixmap_item_) {
        pixmap_item_ = new QGraphicsPixmapItem();
        scene_->addItem(pixmap_item_);
    }

    pixmap_item_->setPixmap(high_qual);
    scene_->setSceneRect(high_qual.rect());
    // Synchronize view
    resetTransform();

    // Re-center view on the same relative image coordinates
    const double target_x = rel_x * scene_->width();
    const double target_y = rel_y * scene_->height();

    if (!std::isnan(target_x) && !std::isnan(target_y)) {
        centerOn(target_x, target_y);
    }
}