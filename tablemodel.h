#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "ingredient.h"

#include "nlohmann/json.hpp"

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>

#include <vector>
#include <tuple>

using namespace nlohmann;

struct AbstractProduct;
class ProductDictionary;

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    using product_list_type = std::vector<std::tuple<std::string, ProductParams, double, bool>>;

    explicit TableModel(ProductDictionary & dict, bool editable = true, QObject * parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // My API
    void append(const TableModel * other);

    bool create_row(int position, const QModelIndex &parent = QModelIndex());

    template<typename S, typename P>
    bool emplace_row(int position, S&& row_name, P&& row_data, double weight = default_weight, bool is_bold = false, const QModelIndex &parent = QModelIndex())
    {
        beginInsertRows(parent, position, position);

        bool res { true };
        if(product_list.empty() && position == 0)
        {
            product_list.emplace_back(std::forward<S>(row_name), std::forward<P>(row_data), weight, is_bold);
        }
        else if(static_cast<std::size_t>(position) <= product_list.size())
        {
            product_list.emplace(product_list.begin() + position,
                                std::forward<S>(row_name), std::forward<P>(row_data), weight, is_bold);
        }
        else
        {
            res = false;
        }

        endInsertRows();

        return res;
    }

    bool remove_row(int position, const QModelIndex &parent = QModelIndex());

    json get_json() const;

    ProductParams summary() const;
    ProductParams mean_value() const;
    double total_weight() const;

    void set_editable(bool editable);
    bool is_editable() const;

    void clear();

    static constexpr std::size_t name_idx()
    {
        return 0;
    }

    static constexpr std::size_t weight_idx()
    {
        return 5;
    }

private:
    product_list_type product_list;
    const ProductDictionary & product_dict_ref;
    bool editable;

    static const double default_weight;
};

#endif // TABLEMODEL_H
