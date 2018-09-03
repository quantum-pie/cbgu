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

}

#endif // TREEUTILS_H
