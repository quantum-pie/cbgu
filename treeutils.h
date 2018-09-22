#ifndef TREEUTILS_H
#define TREEUTILS_H

#include "nlohmann/json.hpp"

#include <QModelIndex>

using namespace nlohmann;

class TreeModel;
class TableModel;
class ProductDictionary;
class CheckableListModel;

namespace treeutils
{

TreeModel * build_tree(TreeModel * tree_model, ProductDictionary & dict,
                  const json & j, const QModelIndex & index = QModelIndex{});

TableModel * build_table(TableModel * table_model, const json & j);

CheckableListModel * build_list(CheckableListModel * list_model, const json & j, bool patch);

void dictionary_item_renamed(ProductDictionary & dict, const QString & old_name);

void empty_name_error();

void same_name_error();

void is_used_error(const QString & name);

void is_added_error();

void empty_composition_error();

bool delete_question();

std::string date_to_string(const QDate & date);

QString format_number(double number);

}

#endif // TREEUTILS_H
