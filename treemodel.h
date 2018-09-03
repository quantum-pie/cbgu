#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "nlohmann/json.hpp"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include <memory>

class AbstractTreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(const nlohmann::json & structure, QObject * parent = nullptr);
    ~TreeModel() override;

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
    void rebuild(const nlohmann::json & structure);

    // debug
    QModelIndex index(AbstractTreeItem * item) const;

private:
    AbstractTreeItem * get_item(const QModelIndex &index) const;
    AbstractTreeItem * root_item;
};

#endif // TREEMODEL_H
