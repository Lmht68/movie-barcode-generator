#pragma once

#include <QDockWidget>

class QTreeView;
class QFileSystemModel;
class QStackedWidget;
class QPushButton;
class QModelIndex;

class FileBrowserDock : public QDockWidget {
    Q_OBJECT

public:
    explicit FileBrowserDock(QWidget *parent = nullptr);

    void OpenFolder(const QString &path);
    void ClearFolder();

signals:
    void RequestOpenFolder();
    void FileSelected(const QString &file_path);

private slots:
    void OnItemClicked(const QModelIndex &index);

private:
    QStackedWidget *widget_stacked_;
    QPushButton *btn_open_folder_;
    QPushButton *btn_open_file_;
    QTreeView *view_tree_;
    QFileSystemModel *model_file_system_;
};
