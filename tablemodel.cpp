#include "tablemodel.h"
#include "meal.h"
#include "productdictionary.h"

const double TableModel::default_weight { 100 };

TableModel::TableModel(ProductDictionary & dict, QObject * parent)
    : QAbstractTableModel{ parent }, product_dict_ref{ dict }
{}

int TableModel::columnCount(const QModelIndex &) const
{
    return 6;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant{};

    if(role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant{};

    auto idx { static_cast<std::size_t>(index.row()) };
    auto item = product_list[idx].first;
    auto weight = product_list[idx].second;

    switch(index.column())
    {
    case 0:
        return QString::fromStdString(item->get_name());
    case 1:
        return item->get_calories();
    case 2:
        return item->get_proteins();
    case 3:
        return item->get_fats();
    case 4:
        return item->get_carbs();
    case 5:
        return weight;
    default:
        return QVariant{};
    }
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    auto flg { QAbstractItemModel::flags(index) };
    if(index.column() == 0 || index.column() == 5)
        return Qt::ItemIsEditable | flg;
    else return flg;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            return "Product";
        case 1:
            return "Calories";
        case 2:
            return "Proteins";
        case 3:
            return "Fats";
        case 4:
            return "Carbohydrates";
        case 5:
            return "Weight";
        default:
            return "[No Data]";
        }
    }

    return QVariant{};
}

bool TableModel::create_row(int position, const QModelIndex &parent)
{
    return insert_row(&dummy_product, position, parent);
}

bool TableModel::insert_row(const AbstractProduct * row, int position, const QModelIndex &parent)
{
    beginInsertRows(parent, position, position);

    bool res { true };
    if(product_list.empty() && position == 0)
    {
        product_list.emplace_back(std::make_pair(row, default_weight));
    }
    else if(static_cast<std::size_t>(position) <= product_list.size())
    {
        product_list.insert(product_list.begin() + position, std::make_pair(row, default_weight));
    }
    else
    {
        res = false;
    }

    endInsertRows();

    return res;
}

bool TableModel::remove_row(int position, const QModelIndex &parent)
{
    if(!product_list.empty() && static_cast<std::size_t>(position) < product_list.size())
    {
        beginRemoveRows(parent, position, position);
        product_list.erase(product_list.begin() + position);
        endRemoveRows();
        return true;
    }
    else return false;
}

int TableModel::rowCount(const QModelIndex &) const
{
    return static_cast<int>(product_list.size());
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;

    auto idx { static_cast<std::size_t>(index.row()) };
    switch(index.column())
    {
    case 0:
        {
        auto item = product_dict_ref.get(value.toString().toStdString());
        if(item)
            product_list[idx].first = item;
        break;
        }
    case 5:
        product_list[idx].second = value.toDouble();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    return true;
}
