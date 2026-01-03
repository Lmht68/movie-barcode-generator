#include "utils/logger.h"
#include "gui/mainwindow.h"

#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QMessageBox>

#include <iostream>
#include <filesystem>

int main(int argc, char *argv[])
{
    try
    {
        QApplication app(argc, argv);

        try
        {
            // Initialize spdlog
            app_logger::InitLogger();
            spdlog::info("Movie Barcode Generator starting...");

            // Initialize and show main window
            MainWindow main_window;
            main_window.show();

            return app.exec();
        }
        catch (const std::runtime_error &e)
        {
            QMessageBox::critical(nullptr, "Logger Initialization Error.", e.what());
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(nullptr, "Initialization Error.", e.what());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "CRITICAL FAILURE: Qt GUI Subsystem could not start." << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
}