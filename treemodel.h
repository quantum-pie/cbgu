#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "roottreeitem.h"

#include "nlohmann/json.hpp"

#include <deque>

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

using namespace nlohmann;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject * parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // My API
    bool insert_row(AbstractTreeItem * row, int position, const QModelIndex &parent = QModelIndex());
    bool remove_row(int position, const QModelIndex &parent = QModelIndex());

    bool is_category(const QModelIndex & index) const;

    json get_json() const;

    QModelIndex index(AbstractTreeItem * item) const;

    void clear();
    void apply(bool confirmed);
    void weak_delete(const QModelIndex &index);
    void weak_add(AbstractTreeItem * item, const QModelIndex &index);

signals:
    void which_data_changed(const QVariant & before,
                            const QModelIndex & index);

    void row_hard_removed(const QModelIndex &index);

private:
    AbstractTreeItem * get_item(const QModelIndex &index) const;
    RootTreeItem root_item;

    std::deque<AbstractTreeItem *> die_if_confirmed;
    std::deque<AbstractTreeItem *> die_if_rejected;
};

#endif // TREEMODEL_H
