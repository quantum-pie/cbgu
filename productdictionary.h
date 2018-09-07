#ifndef PRODUCTDICTIONARY_H
#define PRODUCTDICTIONARY_H

#include "sortednames.h"

#include <unordered_map>
#include <memory>

class AbstractProduct;

class ProductDictionary
{
public:
    std::shared_ptr<AbstractProduct> get(const std::string & name) const;
    bool insert(const std::shared_ptr<AbstractProduct> & new_product);
    bool remove(const QString & name);
    QAbstractItemModel * get_names();

private:
    std::unordered_map<std::string, std::weak_ptr<AbstractProduct>> dict_backend;
    SortedNames sorted_names;
};

#endif // PRODUCTDICTIONARY_H
