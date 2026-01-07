#include "gui/main_window.h"
#include "utils/logger.h"

#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>
#include <opencv2/opencv.hpp>

#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        // Forces Qt to read and use system palette on Windows
        qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
        QApplication app(argc, argv);

        try {
            // Init spdlog
            app_logger::InitLogger();
            spdlog::info("Movie Barcode Generator starting...");
            // Init app style
            app.setStyle(QStyleFactory::create("fusion"));
            // Init and show main window
            MainWindow main_window;
            main_window.show();

            return app.exec();
        } catch (const std::runtime_error &e) {
            QMessageBox::critical(nullptr, "Logger Initialization Error.", e.what());
        } catch (const std::exception &e) {
            QMessageBox::critical(nullptr, "Initialization Error.", e.what());
        }
    } catch (const std::exception &e) {
        std::cerr << "CRITICAL FAILURE: Qt GUI Subsystem could not start." << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
}