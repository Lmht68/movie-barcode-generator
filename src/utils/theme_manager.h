#ifndef UTILS_THEME_MANAGER_H
#define UTILS_THEME_MANAGER_H

#include "constants.h"
#include "exceptions.h"

#include <QApplication>
#include <QFile>
#include <QWidget>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windows.h>
#pragma comment(lib, "dwmapi.lib")
#endif

class ThemeManager {
public:
    static void ApplyTheme(Qt::ColorScheme scheme, QWidget *window = nullptr) {
        auto *app = qobject_cast<QApplication *>(QCoreApplication::instance());
        if (!app) return;

        bool is_dark = (scheme == Qt::ColorScheme::Dark);
        QString path_theme = is_dark ? resource_path::kAppStyleDark : resource_path::kAppStyleLight;
        QFile file_theme(path_theme);
        QFile file_main(resource_path::kAppStyle);
        QString css_theme, css_main;

        if (!file_theme.open(QFile::ReadOnly | QFile::Text)) {
            throw app_exception::StyleInitException(
                "Theme CSS file missing: " + file_theme.fileName().toStdString()
            );
        }
        css_theme = QTextStream(&file_theme).readAll();
        file_theme.close();

        if (!file_main.open(QFile::ReadOnly | QFile::Text)) {
            throw app_exception::StyleInitException(
                "App CSS file missing: " + file_main.fileName().toStdString()
            );
        }
        css_main = QTextStream(&file_main).readAll();
        file_main.close();

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
};
#endif