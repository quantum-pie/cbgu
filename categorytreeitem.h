#ifndef CATEGORYTREEITEM_H
#define CATEGORYTREEITEM_H

#include "abstracttreeitem.h"

class CategoryTreeItem : public AbstractTreeItem
{
public:
    template<typename S>
    CategoryTreeItem(S&& name)
        : name{ std::forward<S>(name) } {}

    bool set_data(const QVariant & value, int column) override;
    QVariant data(int column) const override;
    bool is_category() const override;

private:
    json to_json() const override;
    std::string name;
};

#endif // CATEGORYTREEITEM_H
