#include "treeutils.h"
#include "treemodel.h"
#include "categorytreeitem.h"
#include "ingredienttreeitem.h"
#include "mealtreeitem.h"
#include "ingredient.h"
#include "meal.h"
#include "productdictionary.h"

namespace treeutils
{

void rebuild_tree(TreeModel * tree_model, ProductDictionary & dict, const json & j, const QModelIndex & index)
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
            rebuild_tree(tree_model, dict, value, tree_model->index(new_category));
        }
        else if(type == "ingredient")
        {
            auto new_ingredient = new Ingredient(name,
                                                 value["calories"],
                                                 value["proteins"],
                                                 value["fats"],
                                                 value["carbohydrates"]);

            auto new_ingredient_item = new IngredientTreeItem(new_ingredient);
            tree_model->insert_row(new_ingredient_item, child_counter++, index);
            dict.insert(new_ingredient);
        }
        else if(type == "meal")
        {
            Meal::ingredients_container_type ingredients;
            for(auto ing_it = value.begin(); ing_it != value.end(); ++ing_it)
            {
                auto & ing_name = ing_it.key();
                auto & ing_weight = ing_it.value();
                ingredients.emplace_back(dict.find(ing_name), ing_weight);
            }

            auto new_meal = new Meal(name, ingredients);
            auto new_meal_item = new MealTreeItem(new_meal);
            tree_model->insert_row(new_meal_item, child_counter++, index);
            dict.insert(new_meal);
        }
    }
}

}