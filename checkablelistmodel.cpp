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
        return std::get<1>(list[index.row()]) ?
                    Qt::Checked : Qt::Unchecked;
    case Qt::DisplayRole:
        return QString::fromStdString(std::get<0>(list[index.row()]));
    case Qt::BackgroundColorRole:
        return std::get<1>(list[index.row()]) ? std::get<2>(list[index.row()]) : Qt::lightGray;
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
    case Qt::CheckStateRole:
    {
        std::get<1>(list[index.row()]) = (value.toInt() == Qt::Checked);
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
        json j_it;
        j_it["checked"] = std::get<1>(item);
        j_it["color"] = std::get<2>(item).name().toStdString();
        j[std::get<0>(item)] = j_it;
    }
    return j;
}

json CheckableListModel::get_goals() const
{
    json j;
    for(auto & item : list)
    {
        json j_it;
        j_it["checked"] = false;
        j_it["color"] = std::get<2>(item).name().toStdString();
        j[std::get<0>(item)] = j_it;
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

