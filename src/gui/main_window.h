#pragma once

#include "central_widget.h"
#include "file_browser_dock.h"

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
    QAction* action_import_metadata_;
    QAction* action_export_metadata_;
    QAction* action_save_metadata_;
    QAction* action_open_about_dialog_;

    void InitMenuBar();

private slots:
    void ImportMediaFile();
    void ImportMediaFolder();
    void ExportBarcodeImage();

    void ImportMetadata();
    void ExportMetadata();
    void SaveMetadata();

    void OpenAboutDialog();
};