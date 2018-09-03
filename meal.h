#ifndef MEAL_H
#define MEAL_H

#include "abstractproduct.h"

#include <vector>

struct Meal : AbstractProduct
{
public:
    using ingredients_container_type = std::vector<std::pair<const AbstractProduct *, double>>;

    template<typename S, typename A>
    Meal(S&& name, A&& ingredients)
        : AbstractProduct{ std::forward<S>(name) },
          ingredients{ std::forward<A>(ingredients) }
    {}

    template<typename A>
    void set_ingredients(A&& new_ingredients)
    {
        ingredients = std::forward<A>(new_ingredients);
    }

    double get_calories() const override;
    double get_proteins() const override;
    double get_fats() const override;
    double get_carbs() const override;

    ingredients_container_type get_ingredients();
    double total_weight() const;

private:
    ingredients_container_type ingredients;
};

#endif // MEAL_H
