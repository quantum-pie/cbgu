#include "roottreeitem.h"

bool RootTreeItem::set_data(const QVariant &, int)
{
    return false;
}

QVariant RootTreeItem::data(int column) const
{
    switch(column)
    {
    case 0:
        return "Product";
    case 1:
        return "Calories";
    case 2:
        return "Proteins";
    case 3:
        return "Fats";
    case 4:
        return "Carbohydrates";
    default:
        return "[No Data]";
    }
}

bool RootTreeItem::is_category() const
{
    return true;
}

json RootTreeItem::to_json() const
{
    json j;
    j["type"] = "root";
    return j;
}
