#ifndef TREEUTILS_H
#define TREEUTILS_H

#include "nlohmann/json.hpp"

#include <QModelIndex>

using namespace nlohmann;

class TreeModel;
class ProductDictionary;

namespace treeutils
{

void rebuild_tree(TreeModel * tree_model, ProductDictionary & dict,
                  const json & j, const QModelIndex & index = QModelIndex{});

void dictionary_item_renamed(ProductDictionary & dict, const QString & old_name);

void empty_name_error();

void same_name_error();

}

#endif // TREEUTILS_H
