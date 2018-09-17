#include "treeutils.h"
#include "treemodel.h"
#include "tablemodel.h"
#include "checkablelistmodel.h"
#include "categorytreeitem.h"
#include "ingredienttreeitem.h"
#include "mealtreeitem.h"
#include "ingredient.h"
#include "meal.h"
#include "productdictionary.h"

#include <QMessageBox>
#include <QDate>
#include <QIcon>

namespace treeutils
{

TreeModel * build_tree(TreeModel * tree_model, ProductDictionary & dict, const json & j, const QModelIndex & index)
{
    int child_counter {0};
    for(auto it = j.begin(); it != j.end(); ++it)
    {
        auto & type = it.value()["type"];
        auto & value = it.value()["value"];
        auto & name = it.key();

        if(type == "category")
        {
            auto new_category = new CategoryTreeItem(name);
            tree_model->insert_row(new_category, child_counter++, index);
            build_tree(tree_model, dict, value, tree_model->index(new_category));
        }
        else if(type == "ingredient")
        {
            auto new_ingredient = std::make_shared<Ingredient>(name,
                                                 value["calories"],
                                                 value["proteins"],
                                                 value["fats"],
                                                 value["carbohydrates"]);

            dict.insert(new_ingredient);

            auto new_ingredient_item = new IngredientTreeItem(std::move(new_ingredient));
            tree_model->insert_row(new_ingredient_item, child_counter++, index);
        }
        else if(type == "meal")
        {
            Meal::ingredients_container_type ingredients;
            for(auto ing_it = value.begin(); ing_it != value.end(); ++ing_it)
            {
                auto & ing_name = ing_it.key();
                auto & ing_weight = ing_it.value();
                ingredients.emplace_back(dict.get(ing_name), ing_weight);
            }

            auto new_meal = std::make_shared<Meal>(name, ingredients);
            dict.insert(new_meal);
            auto new_meal_item = new MealTreeItem(std::move(new_meal));
            tree_model->insert_row(new_meal_item, child_counter++, index);
        }
    }

    return tree_model;
}

TableModel * build_table(TableModel * table_model, const json & j)
{
    int rows {0};
    table_model->clear();
    if(!j.is_null())
    {
        for(auto it = j.begin(); it != j.end(); ++it)
        {
            auto& name = it.key();
            const json& data = it.value();
            table_model->emplace_row(name,
                                     ProductParams {
                                     data["calories"],
                                     data["proteins"],
                                     data["fats"],
                                     data["carbohydrates"] },
                                     rows);
            table_model->setData(table_model->index(rows++, table_model->weight_idx()),
                                                    data["weight"].get<double>());
        }
    }
    return table_model;
}

CheckableListModel * build_list(CheckableListModel * list_model, const json & j)
{
    int rows {0};
    list_model->clear();
    if(!j.is_null())
    {
        for(auto it = j.begin(); it != j.end(); ++it)
        {
            auto val = it.value();
            auto checked = val["checked"] ? Qt::Checked : Qt::Unchecked;
            list_model->add_row(it.key(), QString::fromStdString(val["color"]));
            list_model->setData(list_model->index(rows++), checked, Qt::CheckStateRole);
        }
    }
    return list_model;
}

void dictionary_item_renamed(ProductDictionary & dict, const QString & old_name)
{
    auto std_old { old_name.toStdString() };
    auto item = dict.get(std_old);
    if(dict.get(item->get_name()))
    {
        /* revert */
        item->set_name(std_old);
        same_name_error();
    }
    else
    {
        dict.remove(old_name);
        dict.insert(item);
    }
}

void empty_name_error()
{
    QMessageBox error_message;
    error_message.setWindowIcon(QIcon(":/icons/icons/warning.png"));
    error_message.setIcon(QMessageBox::Warning);
    error_message.setWindowTitle(QObject::tr("Error"));
    error_message.setText(QObject::tr("Product name cannot be empty"));
    error_message.setFixedSize(500, 200);
    error_message.exec();
}

void same_name_error()
{
    QMessageBox error_message;
    error_message.setWindowIcon(QIcon(":/icons/icons/warning.png"));
    error_message.setIcon(QMessageBox::Warning);
    error_message.setWindowTitle(QObject::tr("Error"));
    error_message.setText(QObject::tr("Product with such name already exists"));
    error_message.setFixedSize(500, 200);
    error_message.exec();
}

void is_used_error(const QString & name)
{
    QMessageBox error_message;
    error_message.setWindowIcon(QIcon(":/icons/icons/error.png"));
    error_message.setIcon(QMessageBox::Critical);
    error_message.setWindowTitle(QObject::tr("Error"));
    error_message.setText("'" + name + QObject::tr("' meal use this ingredient - aborting removal"));
    error_message.setFixedSize(500, 200);
    error_message.exec();
}

void is_added_error()
{
    QMessageBox error_message;
    error_message.setWindowIcon(QIcon(":/icons/icons/warning.png"));
    error_message.setIcon(QMessageBox::Warning);
    error_message.setWindowTitle(QObject::tr("Error"));
    error_message.setText(QObject::tr("Product with such name already added to the list"));
    error_message.setFixedSize(500, 200);
    error_message.exec();
}

void empty_composition_error()
{
    QMessageBox error_message;
    error_message.setWindowIcon(QIcon(":/icons/icons/warning.png"));
    error_message.setIcon(QMessageBox::Warning);
    error_message.setWindowTitle(QObject::tr("Error"));
    error_message.setText(QObject::tr("Meal has no ingredients"));
    error_message.setFixedSize(500, 200);
    error_message.exec();
}

bool delete_question()
{
    QMessageBox question_message;
    question_message.setWindowIcon(QIcon(":/icons/icons/garbage.png"));
    question_message.setIcon(QMessageBox::Question);
    question_message.setWindowTitle(QObject::tr("Remove Item"));
    question_message.setText(QObject::tr("Are you sure you want to remove this item?"));
    question_message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    question_message.setFixedSize(500, 200);
    return question_message.exec() == QMessageBox::Yes;
}

std::string date_to_string(const QDate & date)
{
    return std::to_string(date.day()) + '_' +
           std::to_string(date.month()) + '_' +
           std::to_string(date.year());
}

QString format_number(double number)
{
    return QString::number(number, 'f', 1);
}

}
