#include "meal.h"

Meal::ingredients_container_type Meal::get_ingredients() const
{
    return ingredients;
}

double Meal::get_calories() const
{
    double res{ 0 };
    for(auto& ingredient : ingredients)
    {
        res += ingredient.first->get_calories() * ingredient.second;
    }
    return res / total_weight();
}

double Meal::get_proteins() const
{
    double res{ 0 };
    for(auto& ingredient : ingredients)
    {
        res += ingredient.first->get_proteins() * ingredient.second;
    }
    return res / total_weight();
}

double Meal::get_fats() const
{
    double res{ 0 };
    for(auto& ingredient : ingredients)
    {
        res += ingredient.first->get_fats() * ingredient.second;
    }
    return res / total_weight();
}

double Meal::get_carbs() const
{
    double res{ 0 };
    for(auto& ingredient : ingredients)
    {
        res += ingredient.first->get_carbs() * ingredient.second;
    }
    return res / total_weight();
}

double Meal::total_weight() const
{
    double res{ 0 };
    for(auto& ingredient : ingredients)
    {
        res += ingredient.second;
    }
    return res;
}
