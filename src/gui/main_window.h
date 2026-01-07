#pragma once

#include "central_widget.h"

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow();

private:
    CentralWidget *central_widget_;
};