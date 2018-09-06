#ifndef TREEUTILS_H
#define TREEUTILS_H

#include "nlohmann/json.hpp"

#include <QModelIndex>

using namespace nlohmann;

class TreeModel;
class TableModel;
class ProductDictionary;

namespace treeutils
{

TreeModel * rebuild_tree(TreeModel * tree_model, ProductDictionary & dict,
                  const json & j, const QModelIndex & index = QModelIndex{});

TableModel * rebuild_table(TableModel * tree_model, const json & j);

void dictionary_item_renamed(ProductDictionary & dict, const QString & old_name);

void empty_name_error();

void same_name_error();

void is_used_error(const QString & name);

std::string date_to_string(const QDate & date);

}

#endif // TREEUTILS_H
