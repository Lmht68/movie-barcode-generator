#include "main_window.h"

#include "../utils/constants.h"

#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow() {
    // Set up central widget and dock widgets
    central_widget_ = new CentralWidget(this);
    setCentralWidget(central_widget_);

    setWindowTitle(WINDOW_TITLE);
    setWindowIcon(QIcon(ResourcePath::kAppIcon));
    resize(1280, 720);
    // Center the window on the primary screen
    move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center());
}