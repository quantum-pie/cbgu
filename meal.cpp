#include "meal.h"

Meal::ingredients_container_type Meal::get_ingredients()
{
    return ingredients;
}

double Meal::get_calories() const
{
    double res{ 0 };
    for(auto ingredient : ingredients)
    {
        res += ingredient->get_calories();
    }
    return res / ingredients.size();
}

double Meal::get_proteins() const
{
    double res{ 0 };
    for(auto ingredient : ingredients)
    {
        res += ingredient->get_proteins();
    }
    return res / ingredients.size();
}

double Meal::get_fats() const
{
    double res{ 0 };
    for(auto ingredient : ingredients)
    {
        res += ingredient->get_fats();
    }
    return res / ingredients.size();
}

double Meal::get_carbs() const
{
    double res{ 0 };
    for(auto ingredient : ingredients)
    {
        res += ingredient->get_carbs();
    }
    return res / ingredients.size();
}
