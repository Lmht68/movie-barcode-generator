#pragma once

#include <QWidget>

class QSplitter;
class BarcodeView;

class CentralWidget : public QWidget {
    Q_OBJECT

public:
    CentralWidget(QWidget *parent = nullptr);
    void DisplayBarcode(const QPixmap &pixmap);

private:
    QSplitter *splitter_widget_;
    BarcodeView *view_barcode_;
};