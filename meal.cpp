#include "meal.h"

Meal::ingredients_container_type Meal::get_ingredients()
{
    return ingredients;
}

ProductParams Meal::calc_params(const ingredients_container_type& ingredients)
{
    ProductParams res{ 0, 0, 0, 0 };
    for(auto ingredient : ingredients)
    {
        res.calories += ingredient->get_calories();
        res.proteins += ingredient->get_proteins();
        res.fats += ingredient->get_fats();
        res.carbs += ingredient->get_carbs();
    }

    res.calories /= ingredients.size();
    res.proteins /= ingredients.size();
    res.fats /= ingredients.size();
    res.carbs /= ingredients.size();

    return res;
}
