#include "productdictionary.h"
#include "abstractproduct.h"

AbstractProduct * ProductDictionary::get(const std::string & name) const
{
    auto it = dict_backend.find(name);
    if(it != dict_backend.end())
    {
        return it->second;
    }
    else return nullptr;
}

bool ProductDictionary::insert(AbstractProduct * new_product)
{
    auto res = dict_backend.emplace(new_product->get_name(), new_product);
    if(res.second)
    {
        return sorted_names.insert_row(QString::fromStdString(new_product->get_name()));
    }
    else return false;
}

bool ProductDictionary::remove(const QString & name)
{
    auto elems = dict_backend.erase(name.toStdString());
    if(elems != 0)
    {
        return sorted_names.remove_row(name);
    }
    else return false;
}

QAbstractItemModel * ProductDictionary::get_names()
{
    return &sorted_names;
}
