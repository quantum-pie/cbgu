#ifndef LIBRARYTREEITEM_H
#define LIBRARYTREEITEM_H

#include "ingredient.h"

#include <forward_list>
#include <memory>

enum class ItemType
{
    item,
    category
};

class IngredientTreeItem
{
public:
    explicit IngredientTreeItem(ItemType type, const QVector<QVariant> &data, IngredientTreeItem *parent = 0);
    ~TreeItem();

    IngredientTreeItem * child(int number);
    IngredientTreeItem * parent();

    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);

    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    std::forward_list<IngredientTreeItem *> child_items;
    std::shared_ptr<Ingredient> item_data;
    IngredientTreeItem * parent_item;
};

#endif // LIBRARYTREEITEM_H
