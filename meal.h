#ifndef MEAL_H
#define MEAL_H

#include "abstractproduct.h"

#include <vector>
#include <memory>

struct Meal : AbstractProduct
{
public:
    using ingredients_container_type = std::vector<std::shared_ptr<const AbstractProduct>>;

    template<typename S, typename A>
    Meal(S&& name, A&& ingredients)
        : AbstractProduct{ std::forward<S>(name) },
          params{ calc_params(ingredients) },
          ingredients{ std::forward<A>(ingredients) }
    {}

    template<typename A>
    void set_ingredients(A&& new_ingredients)
    {
        params = calc_params(new_ingredients);
        ingredients = std::forward<A>(new_ingredients);
    }

    ~Meal() override = default;

    double get_calories() const override
    {
        return params.calories;
    }

    double get_proteins() const override
    {
        return params.proteins;
    }

    double get_fats() const override
    {
        return params.fats;
    }

    double get_carbs() const override
    {
        return params.carbs;
    }

    ingredients_container_type get_ingredients();

private:
    ProductParams calc_params(const ingredients_container_type& ingredients);

    ProductParams params;
    ingredients_container_type ingredients;
};

#endif // MEAL_H
