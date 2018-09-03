#ifndef INGREDIENT_H
#define INGREDIENT_H

#include "abstractproduct.h"

struct Ingredient : AbstractProduct
{   
    template<typename S>
    Ingredient(S&& name, double cal, double pro, double fat, double car)
        : AbstractProduct{ std::forward<S>(name) },
          params{ cal, pro, fat, car }
    {}

    template<typename S>
    Ingredient(S&& name)
        : Ingredient{ std::forward<S>(name), 0, 0, 0, 0 }
    {}

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

    void set_calories(double cal)
    {
        params.calories = cal;
    }

    void set_proteins(double pro)
    {
        params.proteins = pro;
    }

    void set_fats(double fat)
    {
        params.fats = fat;
    }

    void set_carbs(double car)
    {
        params.carbs = car;
    }

private:
    ProductParams params;
};

#endif // INGREDIENT_H
