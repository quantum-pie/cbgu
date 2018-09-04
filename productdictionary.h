#ifndef PRODUCTDICTIONARY_H
#define PRODUCTDICTIONARY_H

#include "sortednames.h"

#include <unordered_map>

class AbstractProduct;

class ProductDictionary
{
public:
    AbstractProduct * get(const std::string & name) const;
    bool insert(AbstractProduct * new_product);
    bool remove(const QString & name);
    QAbstractItemModel * get_names();

private:
    std::unordered_map<std::string, AbstractProduct * > dict_backend;
    SortedNames sorted_names;
};

#endif // PRODUCTDICTIONARY_H
