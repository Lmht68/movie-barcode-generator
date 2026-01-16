#pragma once

#include <QWidget>

class QSplitter;
class BarcodeView;

class CentralWidget : public QWidget {
    Q_OBJECT

public:
    CentralWidget(QWidget *parent = nullptr);

private:
    QSplitter *splitter_widget_;
    BarcodeView *barcode_view_;
};