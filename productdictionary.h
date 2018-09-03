#ifndef PRODUCTDICTIONARY_H
#define PRODUCTDICTIONARY_H

#include <map>

class AbstractProduct;

class ProductDictionary
{
public:
    AbstractProduct * find(const std::string & name);
    bool insert(AbstractProduct * new_product);

private:
    std::map<std::string, AbstractProduct * > dict_backend;
};

#endif // PRODUCTDICTIONARY_H
