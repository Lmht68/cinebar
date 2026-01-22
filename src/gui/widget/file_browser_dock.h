#ifndef GUI_FILE_BROWSER_DOCK_H_
#define GUI_FILE_BROWSER_DOCK_H_

#include <QDockWidget>

class FileBrowserModel;
class MediaFilterProxy;

class QTreeView;
class QStackedWidget;
class QPushButton;
class QModelIndex;
class QLabel;

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
    FileBrowserModel *model_file_browser_;
    MediaFilterProxy *proxy_media_filter_;

    QStackedWidget *widget_stacked_;
    QPushButton *btn_open_folder_;
    QTreeView *view_tree_;
    QLabel *label_root_;
};

#endif