#include "file_browser_dock.h"
#include "file_browser_model.h"
#include "media_filter_proxy.h"

#include "../utils/constants.h"

#include <QFileIconProvider>
#include <QLabel>
#include <QModelIndex>
#include <QPushButton>
#include <QStackedWidget>
#include <QTreeView>
#include <QVBoxLayout>

FileBrowserDock::FileBrowserDock(QWidget *parent)
    : QDockWidget(tr("EXPLORER"), parent),
      widget_stacked_(new QStackedWidget(this)),
      btn_open_folder_(new QPushButton(tr("Open Folder"), this)),
      view_tree_(new QTreeView(this)),
      model_file_system_(new FileBrowserModel(this)),
      media_proxy_model_(new MediaFilterProxy(this)),
      label_root_(new QLabel("", this)) {
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

    auto *widget_fs_tree = new QWidget(this);
    auto *layout_fs_tree = new QVBoxLayout(widget_fs_tree);
    auto *widget_fs_header = new QWidget(this);
    auto *layout_fs_header = new QHBoxLayout(widget_fs_header);
    layout_fs_header->setContentsMargins(0, 0, 0, 0);
    layout_fs_header->setSpacing(1);
    layout_fs_header->setAlignment(Qt::AlignLeft);
    auto *label_root_icon = new QLabel(this);
    label_root_icon->setPixmap(QFileIconProvider().icon(QFileIconProvider::Folder).pixmap(16, 16));
    label_root_->setObjectName("fsTreeRoot");
    layout_fs_header->addWidget(label_root_icon);
    layout_fs_header->addWidget(label_root_);
    layout_fs_tree->addWidget(widget_fs_header);
    layout_fs_tree->addWidget(view_tree_);
    widget_stacked_->addWidget(widget_fs_tree);

    media_proxy_model_->setSourceModel(model_file_system_);
    view_tree_->setModel(media_proxy_model_);
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
    QString folder_name = info.isDir() ? info.fileName() : info.dir().dirName();
    label_root_->setText(folder_name.toUpper());
    label_root_->setToolTip(QDir::toNativeSeparators(root_path));

    model_file_system_->setRootPath(root_path);
    QModelIndex source_root_index = model_file_system_->index(root_path);

    if (!source_root_index.isValid()) return;

    QModelIndex proxy_root_index = media_proxy_model_->mapFromSource(source_root_index);

    view_tree_->setRootIndex(proxy_root_index);
    view_tree_->expand(proxy_root_index);

    widget_stacked_->setCurrentIndex(1);
}

void FileBrowserDock::ClearFolder() {
    view_tree_->setRootIndex(QModelIndex());
    widget_stacked_->setCurrentIndex(0);
}

void FileBrowserDock::OnItemClicked(const QModelIndex &index) {
    QModelIndex source_index = media_proxy_model_->mapToSource(index);
    QFileInfo info = model_file_system_->fileInfo(source_index);

    if (info.isFile() && MediaFormat::isMedia(info.suffix())) {
        emit FileSelected(info.absoluteFilePath());
    }
}