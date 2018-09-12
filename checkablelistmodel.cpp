#include "checkablelistmodel.h"

CheckableListModel::CheckableListModel(QObject * parent)
    : QAbstractListModel{ parent }
{}

Qt::ItemFlags CheckableListModel::flags(const QModelIndex& index) const
{
    auto defaultFlags = QAbstractListModel::flags(index);
    if(index.isValid())
    {
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    else return defaultFlags;
}

QVariant CheckableListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant{};

    switch(role)
    {
    case Qt::CheckStateRole:
        return list[index.row()].second ?
                    Qt::Checked : Qt::Unchecked;
    case Qt::DisplayRole:
        return QString::fromStdString(list[index.row()].first);
    default:
        return QVariant{};
    }
}

bool CheckableListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    switch(role)
    {
    case Qt::EditRole:
        list[index.row()].first = value.toString().toStdString();
        emit dataChanged(index, index);
        return true;
    case Qt::CheckStateRole:
    {
        list[index.row()].second = (value.toInt() == Qt::Checked);
        emit dataChanged(index, index);
        return true;
    }
    default:
        return false;
    }
}

int CheckableListModel::rowCount(const QModelIndex &) const
{
    return list.size();
}

bool CheckableListModel::remove_row(int position)
{
    if(!list.empty() && static_cast<std::size_t>(position) < list.size())
    {
        beginRemoveRows(QModelIndex{}, position, position);
        list.erase(list.begin() + position);
        endRemoveRows();
        return true;
    }
    else return false;
}

json CheckableListModel::get_json() const
{
    json j;
    for(auto & item : list)
    {
        j[item.first] = item.second;
    }
    return j;
}

json CheckableListModel::get_goals() const
{
    json j;
    for(auto & item : list)
    {
        j[item.first] = false;
    }
    return j;
}

void CheckableListModel::clear()
{
    while(rowCount())
    {
        remove_row(0);
    }
}

