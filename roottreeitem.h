#ifndef ROOTTREEITEM_H
#define ROOTTREEITEM_H

#include "abstracttreeitem.h"

#include <array>

class RootTreeItem : public AbstractTreeItem
{
public:
    using AbstractTreeItem::AbstractTreeItem;

    bool set_data(const QVariant & value, int column) override;
    QVariant data(int column) const override;
    bool is_category() const override;
};

#endif // ROOTTREEITEM_H
