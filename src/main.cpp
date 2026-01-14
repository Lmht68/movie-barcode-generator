#include "gui/main_window.h"
#include "utils/constants.h"
#include "utils/logger.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QStyleFactory>
#include <QStyleHints>
#include <QTextStream>
#include <opencv2/opencv.hpp>

#include <filesystem>
#include <iostream>
#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windows.h>
#pragma comment(lib, "dwmapi.lib")
#endif

namespace {
    void ApplyTheme(QApplication *app, Qt::ColorScheme scheme, QWidget *window = nullptr) {
        bool is_dark = (scheme == Qt::ColorScheme::Dark);
        QString path_theme = is_dark ? ResourcePath::kAppStyleDark : ResourcePath::kAppStyleLight;
        QFile file_theme(path_theme);
        QFile file_main(ResourcePath::kAppStyle);
        QString css_theme, css_main;

        if (file_theme.open(QFile::ReadOnly | QFile::Text)) {
            css_theme = QTextStream(&file_theme).readAll();
            file_theme.close();
        }

        if (file_main.open(QFile::ReadOnly | QFile::Text)) {
            css_main = QTextStream(&file_main).readAll();
            file_main.close();
        }

        app->setStyleSheet(css_theme + css_main);
        // Windows-specific Title Bar fix
#ifdef Q_OS_WIN
        if (window) {
            HWND hwnd = reinterpret_cast<HWND>(window->winId());
            BOOL use_dark_mode = is_dark ? TRUE : FALSE;
            // Attribute 20 = DWMWA_USE_IMMERSIVE_DARK_MODE
            DwmSetWindowAttribute(hwnd, 20, &use_dark_mode, sizeof(use_dark_mode));

            // Force title bar to refresh color
            SetWindowPos(
                hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
            );
        }
#endif
    }
}  // namespace

int main(int argc, char *argv[]) {
    try {
        // Forces Qt to read and use system palette on Windows
        // qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
        QApplication app(argc, argv);

        try {
            // Init spdlog
            app_logger::InitLogger();
            spdlog::info("Movie Barcode Generator starting...");
            // Init app style
            app.setStyle(QStyleFactory::create("fusion"));
            // Init main window
            MainWindow main_window;
            // Connect to system theme changes while app is running
            QObject::connect(
                app.styleHints(), &QStyleHints::colorSchemeChanged, &main_window,
                [&app, &main_window](Qt::ColorScheme scheme) {
                    spdlog::info("System theme changed, updating UI...");
                    ApplyTheme(&app, scheme, &main_window);
                }
            );

            // Initial theme application on startup
            ApplyTheme(&app, app.styleHints()->colorScheme(), &main_window);

            // Init and show main window
            main_window.show();

            return app.exec();
        } catch (const std::runtime_error &e) {
            QMessageBox::critical(
                nullptr, "Logger Initialization Error.", "Cannot initialize logger."
            );
            std::cerr << e.what() << std::endl;
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