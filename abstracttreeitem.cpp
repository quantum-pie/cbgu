#include "abstracttreeitem.h"

QVariant productd_data(std::shared_ptr<const AbstractProduct> product, int column)
{
    switch(column)
    {
    case 0:
        return QString::fromStdString(product->get_name());
    case 1:
        return product->get_calories();
    case 2:
        return product->get_proteins();
    case 3:
        return product->get_fats();
    case 4:
        return product->get_carbs();
    default:
        return QVariant{};
    }
}

AbstractTreeItem::AbstractTreeItem()
    : parent_item{ nullptr } {}

AbstractTreeItem::~AbstractTreeItem()
{
    for(auto child : child_items)
    {
        delete child;
    }
}

void AbstractTreeItem::set_parent(AbstractTreeItem * new_parent)
{
    parent_item = new_parent;
}

AbstractTreeItem * AbstractTreeItem::child(int number) const
{
    auto idx = static_cast<std::size_t>(number);
    if(idx < child_items.size())
        return child_items[idx];
    else return nullptr;
}

AbstractTreeItem * AbstractTreeItem::parent() const
{
    return parent_item;
}

int AbstractTreeItem::child_count() const
{
    return static_cast<int>(child_items.size());
}

int AbstractTreeItem::child_number() const
{
    if(parent_item)
    {
        auto parent_children_begin = parent_item->child_items.begin();
        auto parent_children_end = parent_item->child_items.end();
        return static_cast<int>(
                    std::find_if(parent_children_begin, parent_children_end,
                                 [this](auto p){ return p == this; })
                    - parent_children_begin);
    }

    return 0;
}

bool AbstractTreeItem::insert_child(AbstractTreeItem * item, int position)
{
    bool res { true };
    if(child_items.empty() && position == 0)
    {
        item->set_parent(this);
        child_items.emplace_back(item);
    }
    else if(static_cast<std::size_t>(position) <= child_items.size())
    {
        item->set_parent(this);
        child_items.insert(child_items.begin() + position, item);
    }
    else
    {
        res = false;
    }

    return res;
}

bool AbstractTreeItem::remove_child(int position)
{
    if(!child_items.empty() && static_cast<std::size_t>(position) < child_items.size())
    {
        delete child_items[position];
        child_items.erase(child_items.begin() + position);
        return true;
    }
    else return false;
}

json AbstractTreeItem::get_json() const
{
    auto sub_items = to_json();
    for(auto child : child_items)
    {
        sub_items["value"].emplace(child->data(0).toString().toStdString(), child->get_json());
    }
    return sub_items;
}

json AbstractTreeItem::to_json() const
{
    return {};
}

bool AbstractTreeItem::is_editable(int column) const
{
    return column == 0;
}
