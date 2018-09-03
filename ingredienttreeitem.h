#ifndef INGREDIENTTREEITEM_H
#define INGREDIENTTREEITEM_H

#include "abstracttreeitem.h"
#include "ingredient.h"

class IngredientTreeItem : public AbstractTreeItem
{
public:
    explicit IngredientTreeItem(Ingredient * data);
    ~IngredientTreeItem() override;

    bool is_editable(int) const override;
    QVariant data(int column) const override;
    bool set_data(const QVariant & value, int column) override;
    bool is_category() const override;

private:
    Ingredient * item_data;
};

#endif // INGREDIENTTREEITEM_H
