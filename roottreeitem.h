#ifndef ROOTTREEITEM_H
#define ROOTTREEITEM_H

#include "abstracttreeitem.h"

class RootTreeItem : public AbstractTreeItem
{
public:
    using AbstractTreeItem::AbstractTreeItem;

    bool set_data(const QVariant & value, int column) override;
    QVariant data(int column) const override;
    bool is_category() const override;

private:
    json to_json() const override;
};

#endif // ROOTTREEITEM_H
