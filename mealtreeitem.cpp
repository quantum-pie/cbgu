#include "mealtreeitem.h"

MealTreeItem::MealTreeItem(Meal * data)
        : item_data{ data } {}

MealTreeItem::~MealTreeItem()
{
    delete item_data;
}

QVariant MealTreeItem::data(int column) const
{
    return productd_data(item_data, column);
}

bool MealTreeItem::set_data(const QVariant & value, int column)
{
    if(is_editable(column))
    {
        item_data->set_name(value.toString().toStdString());
        return true;
    }
    else return false;
}

bool MealTreeItem::is_category() const
{
    return false;
}

json MealTreeItem::to_json() const
{
    json ingredients;
    ingredients["type"] = "meal";
    for(auto& ingredient : item_data->get_ingredients())
    {
        ingredients["value"].emplace(ingredient.first->get_name(), ingredient.second);
    }
    return ingredients;
}
