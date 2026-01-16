#include "barcode_view.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

BarcodeView::BarcodeView(QWidget *parent)
    : QGraphicsView(parent), scene_(new QGraphicsScene(this)), pixmap_item_(nullptr) {
    setScene(scene_);
    // Init QGraphicsView properties
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setBackgroundBrush(QColor(70, 70, 70));
    setFrameStyle(QFrame::NoFrame);
};