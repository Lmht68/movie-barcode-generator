#include "gui/main_window.h"
#include "utils/constants.h"
#include "utils/exceptions.h"
#include "utils/logger.h"
#include "utils/theme_manager.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QStyleFactory>
#include <QStyleHints>
#include <QTextStream>
#include <opencv2/opencv.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
    try {
        // Forces Qt to read and use system palette on Windows
        // qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
        // Tell Qt to handle scaling, not the OS
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        // Ensure pixmaps use the high-res versions
        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        // Init app object
        QApplication app(argc, argv);

        try {
            // Init spdlog
            app_logger::InitLogger();
            spdlog::info("Movie Barcode Generator starting...");
            // Init app style
            app.setStyle(QStyleFactory::create("fusion"));
            // Init main window
            MainWindow window_main;
            // Connect to system theme changes while app is running
            QObject::connect(
                app.styleHints(), &QStyleHints::colorSchemeChanged, &window_main,
                [&app, &window_main](Qt::ColorScheme scheme) {
                    spdlog::info("System theme changed, updating UI...");
                    ThemeManager::ApplyTheme(scheme, &window_main);
                }
            );

            // Initial theme application on startup
            ThemeManager::ApplyTheme(app.styleHints()->colorScheme(), &window_main);

            // Init and show main window
            window_main.show();

            return app.exec();
        } catch (const app_exception::LoggerInitException &e) {
            QMessageBox::critical(
                nullptr, "Logger Initialization Error.", "Cannot initialize logger."
            );
            std::cerr << e.what() << std::endl;
        } catch (const app_exception::StyleInitException &e) {
            QMessageBox::critical(
                nullptr, "Styling Initialization Error.", "Cannot initialize style sheets."
            );
            spdlog::error("UI Styling Failed: {}", e.what());
        } catch (const std::exception &e) {
            QMessageBox::critical(
                nullptr, "Initialization Error.", "Cannot initialize application."
            );
            spdlog::critical("Initialization Error: {}", e.what());
        }
    } catch (const std::exception &e) {
        std::cerr << "CRITICAL FAILURE: Qt GUI Subsystem could not start." << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
}