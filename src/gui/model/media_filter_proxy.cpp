#include "gui/model/media_filter_proxy.h"

#include "utils/constants.h"

MediaFilterProxy::MediaFilterProxy(QObject *parent) : QSortFilterProxyModel(parent) {}

void MediaFilterProxy::setSourceModel(QAbstractItemModel *sourceModel) {
    fs_model_ = qobject_cast<QFileSystemModel *>(sourceModel);
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

[[nodiscard]] bool MediaFilterProxy::filterAcceptsRow(
    int source_row, const QModelIndex &source_parent
) const {
    if (!fs_model_) return true;

    QModelIndex index = fs_model_->index(source_row, 0, source_parent);

    if (fs_model_->isDir(index)) {
        return true;
    }

    return media_format::IsMedia(fs_model_->fileInfo(index).suffix());
}