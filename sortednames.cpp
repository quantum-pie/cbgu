#include "sortednames.h"

SortedNames::SortedNames(QObject * parent)
    : QAbstractListModel{ parent }
{}

QVariant SortedNames::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant{};

    if(role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant{};

    return names[static_cast<std::size_t>(index.row())];
}

bool SortedNames::insert_row(const QString & row)
{
    auto it = std::upper_bound(names.begin(), names.end(), row);
    auto idx = static_cast<int>(it - names.begin());

    beginInsertRows(QModelIndex{}, idx, idx);

    if(names.empty())
        names.emplace_back(row);
    else
        names.insert(it, row);

    endInsertRows();

    return true;
}

bool SortedNames::remove_row(const QString & row)
{
    bool res{ false };
    if(!names.empty())
    {
        auto it_pair = std::equal_range(names.begin(), names.end(), row);
        if(it_pair.first != it_pair.second)
        {
            int idx = static_cast<int>(it_pair.first - names.begin());
            beginRemoveRows(QModelIndex{}, idx, idx);
            names.erase(it_pair.first);
            endRemoveRows();
            res = true;
        }
    }
    return res;
}

int SortedNames::rowCount(const QModelIndex &) const
{
    return static_cast<int>(names.size());
}

