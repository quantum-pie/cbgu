#ifndef MEALTREEITEM_H
#define MEALTREEITEM_H

#include "abstracttreeitem.h"
#include "meal.h"

class MealTreeItem : public AbstractTreeItem
{
public:
    explicit MealTreeItem(Meal * data);
    ~MealTreeItem() override;

    QVariant data(int column) const override;
    bool set_data(const QVariant & value, int column) override;
    bool is_category() const override;

private:
    Meal * item_data;
};

#endif // MEALTREEITEM_H
