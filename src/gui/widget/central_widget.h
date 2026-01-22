#ifndef GUI_WIDGET_CENTRAL_WIDGET_H_
#define GUI_WIDGET_CENTRAL_WIDGET_H_

#include <QWidget>

class QLabel;
class QPushButton;
class BarcodeView;

class CentralWidget : public QWidget {
    Q_OBJECT

public:
    CentralWidget(QWidget *parent = nullptr);
    void DisplayBarcode(const QPixmap &pixmap);
    void UpdateImageInfo(double zoom_level, const QSize &image_size);

private slots:
    void OnFitToScreenClicked();

private:
    BarcodeView *view_barcode_;
    QLabel *label_zoom_level_;
    QLabel *label_image_dimension_;
    QPushButton *btn_fit_to_screen_;
    QWidget *widget_view_info_;
};

#endif