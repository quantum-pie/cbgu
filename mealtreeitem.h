#ifndef MEALTREEITEM_H
#define MEALTREEITEM_H

#include "abstracttreeitem.h"
#include "meal.h"

#include <memory>

class MealTreeItem : public AbstractTreeItem
{
public:
    explicit MealTreeItem(std::shared_ptr<Meal> data);
    ~MealTreeItem() override = default;

    QVariant data(int column) const override;
    bool set_data(const QVariant & value, int column) override;
    bool is_category() const override;

private:
    json to_json() const override;

    std::shared_ptr<Meal> item_data;
};

#endif // MEALTREEITEM_H
