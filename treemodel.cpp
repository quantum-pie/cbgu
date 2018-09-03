#include "treemodel.h"
#include "roottreeitem.h"

TreeModel::TreeModel(const nlohmann::json & structure, QObject * parent)
    : QAbstractItemModel{ parent }
{
    // TODO parse json and build tree
    root_item = new RootTreeItem;
}

TreeModel::~TreeModel()
{
    delete root_item;
}

int TreeModel::columnCount(const QModelIndex &) const
{
    return root_item->column_count();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant{};

    if(role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant{};

    auto item = get_item(index);

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    Qt::ItemFlags flg { QAbstractItemModel::flags(index) };
    if(get_item(index)->is_editable(index.column()))
        return Qt::ItemIsEditable | flg;
    else return flg;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return root_item->data(section);

    return QVariant{};
}

AbstractTreeItem * TreeModel::get_item(const QModelIndex &index) const
{
    if(index.isValid())
    {
        auto item = static_cast<AbstractTreeItem *>(index.internalPointer());
        if(item)
            return item;
    }
    return root_item;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex{};

    AbstractTreeItem * parent_item = get_item(parent);

    auto child_item = parent_item->child(row);
    if(child_item)
        return createIndex(row, column, child_item);
    else return QModelIndex{};
}

bool TreeModel::insert_row(AbstractTreeItem * row, int position, const QModelIndex &parent)
{
    AbstractTreeItem * parent_item = get_item(parent);
    bool success;

    beginInsertRows(parent, position, position);
    success = parent_item->insert_child(row, position);
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex{};

    AbstractTreeItem * child_item = get_item(index);
    auto parent_item = child_item->parent();

    if(parent_item == root_item)
        return QModelIndex{};

    return createIndex(static_cast<int>(parent_item->child_number()), 0, parent_item);
}

QModelIndex TreeModel::index(AbstractTreeItem * item) const
{
    return createIndex(item->child_number(), 0, item);
}

bool TreeModel::is_category(const QModelIndex &index) const
{
    if(!index.isValid())
        return false;

    return get_item(index)->is_category();
}

bool TreeModel::remove_row(int position, const QModelIndex &parent)
{
    AbstractTreeItem * parent_item = get_item(parent);
    bool success = true;

    beginRemoveRows(parent, position, position);
    success = parent_item->remove_child(position);
    endRemoveRows();

    return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    AbstractTreeItem * parent_item = get_item(parent);

    return parent_item->child_count();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;

    AbstractTreeItem * item = get_item(index);
    bool result = item->set_data(value, index.column());

    if(result)
        emit dataChanged(index, index);

    return result;
}
