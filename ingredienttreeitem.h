#ifndef INGREDIENTTREEITEM_H
#define INGREDIENTTREEITEM_H

#include "abstracttreeitem.h"
#include "ingredient.h"

#include <memory>

class IngredientTreeItem : public AbstractTreeItem
{
public:
    explicit IngredientTreeItem(std::shared_ptr<Ingredient> data);
    ~IngredientTreeItem() override = default;

    bool is_editable(int) const override;
    QVariant data(int column) const override;
    bool set_data(const QVariant & value, int column) override;
    bool is_category() const override;

private:
    json to_json() const override;

    std::shared_ptr<Ingredient> item_data;
};

#endif // INGREDIENTTREEITEM_H
