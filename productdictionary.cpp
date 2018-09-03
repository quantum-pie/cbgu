#include "productdictionary.h"
#include "abstractproduct.h"

AbstractProduct * ProductDictionary::find(const std::string & name)
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
    return res.second;
}
