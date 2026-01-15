#include "file_browser_dock.h"

#include <QFileSystemModel>
#include <QLabel>
#include <QModelIndex>
#include <QPushButton>
#include <QStackedWidget>
#include <QTreeView>
#include <QVBoxLayout>

namespace {
    bool IsVideoFile(const QFileInfo &file_info) {
        static const QStringList video_extensions = {"mp4", "avi", "mkv", "mov"};
        return video_extensions.contains(file_info.suffix().toLower());
    }
}  // namespace

FileBrowserDock::FileBrowserDock(QWidget *parent)
    : QDockWidget(tr("EXPLORER"), parent),
      widget_stacked_(new QStackedWidget(this)),
      btn_open_folder_(new QPushButton(tr("Open Folder"), this)),
      view_tree_(new QTreeView(this)),
      model_file_system_(new QFileSystemModel(this)) {
    setAllowedAreas(Qt::LeftDockWidgetArea);
    auto *dock_title_label = new QLabel(tr("EXPLORER"), this);
    dock_title_label->setObjectName("dockTitle");
    dock_title_label->setContentsMargins(8, 8, 0, 0);
    dock_title_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setTitleBarWidget(dock_title_label);

    auto *widget_placeholder = new QWidget(widget_stacked_);
    auto *layout = new QVBoxLayout(widget_placeholder);
    btn_open_folder_->setObjectName("btnSelectFolder");
    btn_open_folder_->setMaximumWidth(300);
    layout->addWidget(new QLabel(tr("You have not yet opened a media folder."), this));
    layout->addWidget(btn_open_folder_);
    layout->addStretch();
    widget_stacked_->addWidget(widget_placeholder);

    connect(btn_open_folder_, &QPushButton::clicked, this, &FileBrowserDock::RequestOpenFolder);

    model_file_system_->setRootPath(QString());
    view_tree_->setModel(model_file_system_);
    widget_stacked_->addWidget(view_tree_);
    view_tree_->setHeaderHidden(true);
    view_tree_->setIndentation(14);
    view_tree_->setUniformRowHeights(true);
    view_tree_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view_tree_->setSelectionMode(QAbstractItemView::SingleSelection);
    view_tree_->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Hide everything except name
    for (int i = 1; i < model_file_system_->columnCount(); ++i) view_tree_->hideColumn(i);

    connect(view_tree_, &QTreeView::clicked, this, &FileBrowserDock::OnItemClicked);

    widget_stacked_->setCurrentIndex(0);
    setWidget(widget_stacked_);
}

void FileBrowserDock::OpenFolder(const QString &path) {
    if (path.isEmpty()) return;

    QFileInfo info(path);
    QString root_path = info.isDir() ? info.absoluteFilePath() : info.absolutePath();

    QModelIndex root_index = model_file_system_->index(root_path);

    if (!root_index.isValid()) return;

    view_tree_->setRootIndex(root_index);
    view_tree_->expand(root_index);
    widget_stacked_->setCurrentIndex(1);
}

void FileBrowserDock::ClearFolder() {
    view_tree_->setRootIndex(QModelIndex());
    widget_stacked_->setCurrentIndex(0);
}

void FileBrowserDock::OnItemClicked(const QModelIndex &index) {
    QFileInfo info = model_file_system_->fileInfo(index);

    if (info.isFile() && IsVideoFile(info)) emit FileSelected(info.absoluteFilePath());
}