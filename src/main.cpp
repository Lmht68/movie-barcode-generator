#include <iostream>
#include <QApplication>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    std::cout << "Qt version: " << QT_VERSION << std::endl;
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    QApplication app(argc, argv);
    return app.exec();
}