#include "gui/widget/central_widget.h"

#include "gui/view/barcode_view.h"

#include <QSplitter>
#include <QVBoxLayout>

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent) {
    // Init central widget layout
    setLayout(new QVBoxLayout(this));
    // Init central widget components
    splitter_widget_ = new QSplitter(Qt::Vertical, this);
    layout()->addWidget(splitter_widget_);
    view_barcode_ = new BarcodeView(this);
    splitter_widget_->addWidget(view_barcode_);
}

void CentralWidget::DisplayBarcode(const QPixmap &pixmap) { view_barcode_->DisplayImage(pixmap); }