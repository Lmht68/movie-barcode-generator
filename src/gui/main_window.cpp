#include "gui/main_window.h"

#include "utils/constants.h"
#include "utils/logger.h"
#include "utils/theme_manager.h"

#include <QAction>
#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>
#include <QUrl>

MainWindow::MainWindow() : QMainWindow(nullptr) {
    InitMenuBar();
    InitCentralWidget();
    InitFileBrowserDockWidget();

    setWindowTitle(APP_NAME);
    setWindowIcon(QIcon(resource_path::kAppIcon));
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

    // View menu
    QMenu* menu_view = menuBar()->addMenu(tr("&View"));

    QAction* action_toggle_theme = menu_view->addAction(tr("Toggle Dark Mode"));
    action_toggle_theme->setCheckable(true);

    action_toggle_theme->setChecked(this->palette().window().color().value() < 128);

    connect(action_toggle_theme, &QAction::toggled, this, [this](bool checked) {
        Qt::ColorScheme scheme = checked ? Qt::ColorScheme::Dark : Qt::ColorScheme::Light;
        ThemeManager::ApplyTheme(scheme, this);
    });

    // Help menu
    QMenu* menu_help = menuBar()->addMenu(tr("&Help"));

    action_open_about_dialog_ = new QAction(tr("About"), this);
    connect(action_open_about_dialog_, &QAction::triggered, this, &MainWindow::OpenAboutDialog);

    menu_help->addAction(action_open_about_dialog_);
    menu_help->addAction(tr("About &Qt"), this, &QApplication::aboutQt);
}

void MainWindow::InitFileBrowserDockWidget() {
    dock_file_browser_ = new FileBrowserDock(this);
    dock_file_browser_->setFeatures(
        QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
    );

    connect(
        dock_file_browser_, &FileBrowserDock::RequestOpenFolder, this,
        &MainWindow::ImportMediaFolder
    );
    connect(
        dock_file_browser_, &FileBrowserDock::FileSelected, this, &MainWindow::DisplayImageFile
    );

    addDockWidget(Qt::LeftDockWidgetArea, dock_file_browser_);
}

void MainWindow::InitCentralWidget() {
    widget_central_ = new CentralWidget(this);
    setCentralWidget(widget_central_);
}

// ===== Slots =====

void MainWindow::ImportMediaFolder() {
    const QString folder = QFileDialog::getExistingDirectory(this, tr("Import Media Folder"));
    if (folder.isEmpty()) return;
    dock_file_browser_->OpenFolder(folder);
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
        this, tr("About CineBar"),
        tr("<b>%1</b><br>"
           "Generate visual color barcodes from movies.<br><br>"
           "<a href='https://github.com/Lmht68/movie-barcode-generator'>"
           "GitHub Repository</a>")
            .arg(APP_DETAIL)
    );
}

void MainWindow::DisplayImageFile(const QString& file_path) {
    QPixmap pixmap(file_path);

    if (pixmap.isNull()) {
        spdlog::warn("Failed to load image from: {}", file_path.toStdString());
        return;
    }

    widget_central_->DisplayBarcode(pixmap);
}