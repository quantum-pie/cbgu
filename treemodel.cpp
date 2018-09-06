#include "treemodel.h"
#include "roottreeitem.h"

TreeModel::TreeModel(QObject * parent)
    : QAbstractItemModel{ parent }, root_item{}
{}

int TreeModel::columnCount(const QModelIndex &) const
{
    return root_item.column_count();
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
        return Qt::NoItemFlags;

    auto item = get_item(index);
    while(item != &root_item)
    {
        auto it = std::find(die_if_confirmed.begin(), die_if_confirmed.end(), item);
        if(it != die_if_confirmed.end())
        {
            return Qt::NoItemFlags;
        }
        item = item->parent();
    }

    item = get_item(index);
    auto flg { QAbstractItemModel::flags(index) };
    if(item->is_editable(index.column()))
        return Qt::ItemIsEditable | flg;
    else return flg;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return root_item.data(section);

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
    return const_cast<RootTreeItem *>(&root_item);
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex{};

    auto parent_item = get_item(parent);

    auto child_item = parent_item->child(row);
    if(child_item)
        return createIndex(row, column, child_item);
    else return QModelIndex{};
}

bool TreeModel::insert_row(AbstractTreeItem * row, int position, const QModelIndex &parent)
{
    auto parent_item = get_item(parent);
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

    auto child_item = get_item(index);
    auto parent_item = child_item->parent();

    if(parent_item == &root_item)
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
    auto parent_item = get_item(parent);
    bool success = true;

    beginRemoveRows(parent, position, position);
    success = parent_item->remove_child(position);
    endRemoveRows();

    return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    auto parent_item = get_item(parent);
    return parent_item->child_count();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;

    auto item = get_item(index);
    auto prev = item->data(index.column());

    bool result { true };
    if(value != prev)
    {
        result = item->set_data(value, index.column());
        if(result)
        {
            emit which_data_changed(prev, index);
            emit dataChanged(index, index);
        }
    }
    return result;
}

json TreeModel::get_json() const
{
    return root_item.get_json();
}

void TreeModel::clear()
{
    while(hasChildren())
    {
        root_item.remove_child(0);
    }
}

void TreeModel::apply(bool confirmed)
{
    auto& container = confirmed ? die_if_confirmed : die_if_rejected;
    for(auto item : container)
    {
        auto idx = index(item);
        if(remove_row(idx.row(), idx.parent()))
        {
            emit row_hard_removed(idx);
        }
    }

    die_if_confirmed.clear();
    die_if_rejected.clear();
}

void TreeModel::weak_delete(const QModelIndex &index)
{
    die_if_confirmed.push_back(get_item(index));
}

void TreeModel::weak_add(AbstractTreeItem * item, const QModelIndex &index)
{
    insert_row(item, 0, index);
    die_if_rejected.push_front(item);
}
