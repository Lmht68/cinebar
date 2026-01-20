#ifndef GUI_MODEL_MEDIA_FILTER_PROXY_H_
#define GUI_MODEL_MEDIA_FILTER_PROXY_H_

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

#endif