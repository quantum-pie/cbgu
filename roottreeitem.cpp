#include "roottreeitem.h"

#include <QObject>

bool RootTreeItem::set_data(const QVariant &, int)
{
    return false;
}

QVariant RootTreeItem::data(int column) const
{
    switch(column)
    {
    case 0:
        return QObject::tr("Product");
    case 1:
        return QObject::tr("Calories");
    case 2:
        return QObject::tr("Proteins");
    case 3:
        return QObject::tr("Fats");
    case 4:
        return QObject::tr("Carbohydrates");
    default:
        return QObject::tr("[No Data]");
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
