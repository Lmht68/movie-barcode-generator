#pragma once

#include <QFileSystemModel>

class FileBrowserModel : public QFileSystemModel {
    Q_OBJECT

public:
    explicit FileBrowserModel(QObject *parent = nullptr, const QString &rootPath = {});

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
};