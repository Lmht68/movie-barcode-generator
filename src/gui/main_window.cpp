#include "main_window.h"

#include "../utils/constants.h"

#include <QAction>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>
#include <QUrl>

MainWindow::MainWindow() : QMainWindow(nullptr) {
    // Set up menu bar
    InitMenuBar();
    // Set up central widget and dock widgets
    central_widget_ = new CentralWidget(this);
    setCentralWidget(central_widget_);

    file_browser_dock_ = new FileBrowserDock(this);
    file_browser_dock_->setFeatures(
        QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
    );
    addDockWidget(Qt::LeftDockWidgetArea, file_browser_dock_);

    setWindowTitle(WINDOW_TITLE);
    setWindowIcon(QIcon(ResourcePath::kAppIcon));
    resize(1600, 900);
    // Center the window on the primary screen
    move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center());
}

void MainWindow::InitMenuBar() {
    // File menu
    QMenu* menu_file = menuBar()->addMenu(tr("&File"));

    action_import_media_folder_ = new QAction(tr("Import Media Folder…"), this);
    action_export_barcode_image_ = new QAction(tr("Export Barcode Image…"), this);
    action_import_preset_ = new QAction(tr("Import Preset…"), this);
    action_export_preset_ = new QAction(tr("Export Preset…"), this);
    action_save_preset_ = new QAction(tr("Save Preset"), this);

    connect(action_import_media_folder_, &QAction::triggered, this, &MainWindow::ImportMediaFolder);
    connect(
        action_export_barcode_image_, &QAction::triggered, this, &MainWindow::ExportBarcodeImage
    );
    connect(action_import_preset_, &QAction::triggered, this, &MainWindow::ImportPreset);
    connect(action_export_preset_, &QAction::triggered, this, &MainWindow::ExportPreset);
    connect(action_save_preset_, &QAction::triggered, this, &MainWindow::SavePreset);

    menu_file->addAction(action_import_media_folder_);
    menu_file->addAction(action_export_barcode_image_);
    menu_file->addSeparator();
    menu_file->addAction(action_import_preset_);
    menu_file->addAction(action_export_preset_);
    menu_file->addAction(action_save_preset_);

    // Help menu
    QMenu* help_menu = menuBar()->addMenu(tr("&Help"));

    action_open_about_dialog_ = new QAction(tr("About"), this);
    connect(action_open_about_dialog_, &QAction::triggered, this, &MainWindow::OpenAboutDialog);
    help_menu->addAction(action_open_about_dialog_);
}

// ===== Slots =====

void MainWindow::ImportMediaFile() {
    const QString file = QFileDialog::getOpenFileName(
        this, tr("Import Media File"), QString(), tr("Video Files (*.mp4 *.mkv *.avi *.mov)")
    );

    if (file.isEmpty()) return;

    // TODO:
}

void MainWindow::ImportMediaFolder() {
    const QString folder = QFileDialog::getExistingDirectory(this, tr("Import Media Folder"));

    if (folder.isEmpty()) return;

    // TODO:
}

void MainWindow::ExportBarcodeImage() {
    const QString file = QFileDialog::getSaveFileName(
        this, tr("Export Barcode Image"), QString(), tr("PNG Image (*.png);;JPEG Image (*.jpg)")
    );

    if (file.isEmpty()) return;

    // TODO:
}

void MainWindow::ImportPreset() {
    const QString file = QFileDialog::getOpenFileName(
        this, tr("Import Preset"), QString(), tr("Preset Files (*.json)")
    );

    if (file.isEmpty()) return;

    // TODO:
}

void MainWindow::ExportPreset() {
    const QString file =
        QFileDialog::getSaveFileName(this, tr("Export Preset"), QString(), tr("JSON (*.json)"));

    if (file.isEmpty()) return;

    // TODO:
}

void MainWindow::SavePreset() {
    // TODO:
}

void MainWindow::OpenAboutDialog() {
    QMessageBox::about(
        this, tr("About MovieBarcode"),
        tr("<b>%1</b><br>"
           "Generate visual color barcodes from movies.<br><br>"
           "<a href='https://github.com/Lmht68/movie-barcode-generator'>"
           "GitHub Repository</a>")
            .arg(WINDOW_TITLE)
    );
}