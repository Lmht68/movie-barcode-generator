#include "gui/model/file_browser_model.h"

#include <QFileIconProvider>

FileBrowserModel::FileBrowserModel(QObject *parent, const QString &rootPath)
    : QFileSystemModel(parent) {
    if (!rootPath.isEmpty()) {
        this->setRootPath(rootPath);
    }
}

[[nodiscard]] QVariant FileBrowserModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return {};

    if (role == Qt::DecorationRole) {
        const QFileInfo file_info = this->fileInfo(index);

        if (file_info.isDir()) {
            return this->iconProvider()->icon(QFileIconProvider::Folder);
        }

        return this->iconProvider()->icon(file_info);
    }

    return QFileSystemModel::data(index, role);
}
