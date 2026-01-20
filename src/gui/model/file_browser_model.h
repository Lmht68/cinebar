#ifndef GUI_MODEL_FILE_BROWSER_MODEL_H_
#define GUI_MODEL_FILE_BROWSER_MODEL_H_

#include <QFileSystemModel>

class FileBrowserModel : public QFileSystemModel {
    Q_OBJECT

public:
    explicit FileBrowserModel(QObject *parent = nullptr, const QString &rootPath = {});

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
};

#endif