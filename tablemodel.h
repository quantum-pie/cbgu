#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "ingredient.h"

#include "nlohmann/json.hpp"

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>

#include <vector>
#include <memory>

using namespace nlohmann;

struct AbstractProduct;
class ProductDictionary;

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(ProductDictionary & dict, QObject * parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // My API
    bool create_row(int position, const QModelIndex &parent = QModelIndex());
    bool insert_row(const std::shared_ptr<const AbstractProduct> & row, int position, const QModelIndex &parent = QModelIndex());
    bool remove_row(int position, const QModelIndex &parent = QModelIndex());

    json get_json() const;

    ProductParams summary() const;

    static constexpr std::size_t name_idx()
    {
        return 0;
    }

    static constexpr std::size_t weight_idx()
    {
        return 5;
    }

private:
    std::vector<std::pair<std::weak_ptr<const AbstractProduct>, double>> product_list;
    const ProductDictionary & product_dict_ref;
    std::shared_ptr<Ingredient> dummy_product;

    static const double default_weight;
};

#endif // TABLEMODEL_H
