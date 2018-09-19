#include "tablemodel.h"
#include "meal.h"
#include "productdictionary.h"
#include "treeutils.h"

#include <QFont>

const double TableModel::default_weight { 100 };

TableModel::TableModel(ProductDictionary & dict, bool editable, QObject * parent)
    : QAbstractTableModel{ parent },
      product_dict_ref{ dict },
      editable{ editable }
{}

int TableModel::columnCount(const QModelIndex &) const
{
    return 6;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant{};

    if(role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::FontRole)
        return QVariant{};

    auto idx { static_cast<std::size_t>(index.row()) };
    auto& name = std::get<0>(product_list[idx]);
    auto& params = std::get<1>(product_list[idx]);
    auto weight = std::get<2>(product_list[idx]);
    auto is_bold = std::get<3>(product_list[idx]);

    if(is_bold && role == Qt::FontRole)
    {
        QFont font;
        font.setBold(true);
        return font;
    }

    double portion { weight / 100.0 };

    switch(index.column())
    {
    case 0:
        return QString::fromStdString(name);
    case 1:
        return treeutils::format_number(params.calories * portion);
    case 2:
        return treeutils::format_number(params.proteins * portion);
    case 3:
        return treeutils::format_number(params.fats * portion);
    case 4:
        return treeutils::format_number(params.carbs * portion);
    case 5:
        return treeutils::format_number(weight);
    default:
        return QVariant{};
    }
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    auto flg { QAbstractItemModel::flags(index) };
    if(editable && ( index.column() == name_idx() || index.column() == weight_idx() ))
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
            return tr("Product");
        case 1:
            return tr("Calories");
        case 2:
            return tr("Proteins");
        case 3:
            return tr("Fats");
        case 4:
            return tr("Carbohydrates");
        case 5:
            return tr("Weight");
        default:
            return tr("[No Data]");
        }
    }

    return QVariant{};
}

void TableModel::append(const TableModel * other)
{
    if(other->rowCount())
    {
        beginInsertRows(QModelIndex{}, rowCount(), rowCount() + other->rowCount() - 1);
        product_list.insert(product_list.end(), other->product_list.begin(), other->product_list.end());
        endInsertRows();
    }
}

bool TableModel::create_row(int position, const QModelIndex &parent)
{
    return emplace_row(position, "", ProductParams{0, 0, 0, 0}, default_weight, false, parent);
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
    case name_idx():
        {
        auto item = product_dict_ref.get(value.toString().toStdString());
        if(item)
        {
            auto it = std::find_if(product_list.begin(), product_list.end(),
            [item](auto & ex_prod)
            {
                return std::get<0>(ex_prod) == item->get_name();
            });
            if(it == product_list.end())
            {
                std::get<0>(product_list[idx]) = item->get_name();
                std::get<1>(product_list[idx]) = {  item->get_calories(),
                                                    item->get_proteins(),
                                                    item->get_fats(),
                                                    item->get_carbs() };
            }
        }
        break;
        }
    case weight_idx():
        std::get<2>(product_list[idx]) = value.toDouble();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

json TableModel::get_json() const
{
    json j;
    for(auto & product : product_list)
    {
        auto& name = std::get<0>(product);
        auto& params = std::get<1>(product);
        auto weight = std::get<2>(product);

        json j_el;
        j_el["calories"] = params.calories;
        j_el["proteins"] = params.proteins;
        j_el["fats"] = params.fats;
        j_el["carbohydrates"] = params.carbs;
        j_el["weight"] = weight;

        j[name] = j_el;
    }
    return j;
}

ProductParams TableModel::summary() const
{
    ProductParams params {0, 0, 0, 0};
    for(auto & product : product_list)
    {
        double portion { std::get<2>(product) / 100.0 };
        auto& par = std::get<1>(product);
        params.calories += par.calories * portion;
        params.proteins += par.proteins * portion;
        params.fats += par.fats * portion;
        params.carbs += par.carbs * portion;
    }
    return params;
}

ProductParams TableModel::mean_value() const
{
    ProductParams params {0, 0, 0, 0};
    double total_w { 0 };
    for(auto & product : product_list)
    {
        double weight { std::get<2>(product) };
        auto& par = std::get<1>(product);
        params.calories += par.calories * weight;
        params.proteins += par.proteins * weight;
        params.fats += par.fats * weight;
        params.carbs += par.carbs * weight;
        total_w += weight;
    }

    if(!product_list.empty())
    {
        params.calories /= total_w;
        params.proteins /= total_w;
        params.fats /= total_w;
        params.carbs /= total_w;
    }
    return params;
}

double TableModel::total_weight() const
{
    double result {0};
    for(auto & product : product_list)
    {
        result += std::get<2>(product);
    }
    return result;
}

void TableModel::clear()
{
    int last = std::max(0, rowCount() - 1);
    beginRemoveRows(QModelIndex{}, 0, last);
    product_list.clear();
    endRemoveRows();
}

void TableModel::set_editable(bool is_editable)
{
    editable = is_editable;
}

bool TableModel::is_editable() const
{
    return editable;
}
