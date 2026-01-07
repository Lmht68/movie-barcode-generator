#pragma once

#include "barcode_view.h"

#include <QSplitter>
#include <QWidget>

class CentralWidget : public QWidget {
    Q_OBJECT

public:
    CentralWidget(QWidget *parent = nullptr);

private:
    QSplitter *splitter_widget_;
    BarcodeView *barcode_view_;
};