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

TreeModel * build_tree(TreeModel * tree_model, ProductDictionary & dict,
                  const json & j, const QModelIndex & index = QModelIndex{});

TableModel * build_table(TableModel * table_model, ProductDictionary & dict, const json & j);

void dictionary_item_renamed(ProductDictionary & dict, const QString & old_name);

void empty_name_error();

void same_name_error();

void is_used_error(const QString & name);

void is_added_error();

std::string date_to_string(const QDate & date);

}

#endif // TREEUTILS_H
