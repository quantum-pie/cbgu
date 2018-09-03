#ifndef ABSTRACTTREEITEM_H
#define ABSTRACTTREEITEM_H

#include "abstractproduct.h"

#include "nlohmann/json.hpp"

#include <vector>

#include <QVariant>

using namespace nlohmann;

QVariant productd_data(const AbstractProduct * product, int column);

class AbstractTreeItem
{
public:
    AbstractTreeItem();
    virtual ~AbstractTreeItem();

    AbstractTreeItem * child(int number) const;
    AbstractTreeItem * parent() const;
    int child_count() const;
    int child_number() const;

    static constexpr int column_count()
    {
        return 5;
    }

    bool insert_child(AbstractTreeItem * item, int position);
    bool remove_child(int position);

    json get_json() const;

    virtual bool is_editable(int column) const;
    virtual bool set_data(const QVariant & value, int column) = 0;
    virtual QVariant data(int column) const = 0;
    virtual bool is_category() const = 0;

private:
    virtual json to_json() const;
    void set_parent(AbstractTreeItem * parent_item);

    std::vector<AbstractTreeItem *> child_items;
    AbstractTreeItem * parent_item;
};

#endif // ABSTRACTTREEITEM_H
