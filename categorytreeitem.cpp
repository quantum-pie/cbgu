#include "categorytreeitem.h"

bool CategoryTreeItem::set_data(const QVariant & value, int column)
{
    if(is_editable(column))
    {
        name = value.toString().toStdString();
        return true;
    }
    else return false;
}

QVariant CategoryTreeItem::data(int column) const
{
    if(is_editable(column))
        return QString::fromStdString(name);
    else
        return QVariant{};
}

bool CategoryTreeItem::is_category() const
{
    return true;
}
