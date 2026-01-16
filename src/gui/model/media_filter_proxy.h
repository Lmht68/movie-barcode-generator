#pragma once

#include <QFileSystemModel>
#include <QSortFilterProxyModel>

class MediaFilterProxy : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit MediaFilterProxy(QObject *parent = nullptr);

    void setSourceModel(QAbstractItemModel *sourceModel) override;

protected:
    [[nodiscard]] bool filterAcceptsRow(
        int source_row, const QModelIndex &source_parent
    ) const override;

private:
    QFileSystemModel *fs_model_;
};