#include <opencv2/opencv.hpp>
#include <QApplication>

#include <iostream>
#include <filesystem>

#include "utils/logger.h"

int main(int argc, char *argv[])
{
    // Initialize spdlog
    app_logger::InitLogger();
    spdlog::info("Movie Barcode Generator starting...");
    spdlog::info("Environment: Qt {} | OpenCV {}", QT_VERSION_STR, CV_VERSION);

    QApplication app(argc, argv);
    return app.exec();
}