#pragma once

#include "gui/widget/central_widget.h"
#include "gui/widget/file_browser_dock.h"

#include <QMainWindow>

class QAction;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow();

private:
    CentralWidget* central_widget_;
    FileBrowserDock* file_browser_dock_;
    QAction* action_import_media_file_;
    QAction* action_import_media_folder_;
    QAction* action_export_barcode_image_;
    QAction* action_import_preset_;
    QAction* action_export_preset_;
    QAction* action_save_preset_;
    QAction* action_open_about_dialog_;

    void InitMenuBar();
    void InitFileBrowserDockWidget();
    void InitCentralWidget();

private slots:
    void ImportMediaFolder();
    void ExportBarcodeImage();

    void ImportPreset();
    void ExportPreset();
    void SavePreset();

    void OpenAboutDialog();

    void DisplayImageFile(const QString& file_path);
};