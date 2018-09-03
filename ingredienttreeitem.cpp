#include "ingredienttreeitem.h"

IngredientTreeItem::IngredientTreeItem(Ingredient * data)
        : item_data{ data } {}

IngredientTreeItem::~IngredientTreeItem()
{
    delete item_data;
}

QVariant IngredientTreeItem::data(int column) const
{
    return productd_data(item_data, column);
}

bool IngredientTreeItem::set_data(const QVariant & value, int column)
{
    switch(column)
    {
    case 0:
        item_data->set_name(value.toString().toStdString());
        return true;
    case 1:
        item_data->set_calories(value.toDouble());
        return true;
    case 2:
        item_data->set_proteins(value.toDouble());
        return true;
    case 3:
        item_data->set_fats(value.toDouble());
        return true;
    case 4:
        item_data->set_carbs(value.toDouble());
        return true;
    default:
        return false;
    }
}

bool IngredientTreeItem::is_editable(int) const
{
    return true;
}

bool IngredientTreeItem::is_category() const
{
    return false;
}

json IngredientTreeItem::to_json() const
{
    json j;
    j["type"] = "ingredient";
    j["value"]["calories"] = item_data->get_calories();
    j["value"]["proteins"] = item_data->get_proteins();
    j["value"]["fats"] = item_data->get_fats();
    j["value"]["carbohydrates"] = item_data->get_carbs();
    return j;
}
