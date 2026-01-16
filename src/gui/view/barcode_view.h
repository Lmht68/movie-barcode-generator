#pragma once

#include <QGraphicsView>

class QGraphicsScene;
class QGraphicsPixmapItem;

class BarcodeView : public QGraphicsView {
    Q_OBJECT

public:
    BarcodeView(QWidget *parent = nullptr);

private:
    QGraphicsScene *scene_;
    QGraphicsPixmapItem *pixmap_item_;
};