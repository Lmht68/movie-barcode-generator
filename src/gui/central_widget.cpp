#include "central_widget.h"

#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
    // Init central widget layout
    setLayout(new QVBoxLayout(this));
    // Init central widget components
    splitter_widget_ = new QSplitter(Qt::Vertical, this);
    layout()->addWidget(splitter_widget_);
    barcode_view_ = new BarcodeView(this);
    splitter_widget_->addWidget(barcode_view_);
}