#ifndef ABSTRACTPRODUCT_H
#define ABSTRACTPRODUCT_H

#include <string>

struct ProductParams
{
    double calories;
    double proteins;
    double fats;
    double carbs;

    ProductParams operator+(const ProductParams & other)
    {
        return
        {
            calories + other.calories,
            proteins + other.proteins,
            fats + other.fats,
            carbs + other.carbs
        };
    }

    void operator+=(const ProductParams & other)
    {
        calories += other.calories;
        proteins += other.proteins;
        fats += other.fats;
        carbs += other.carbs;
    }
};

struct AbstractProduct
{
    template<typename S>
    AbstractProduct(S&& name)
        : name{ std::forward<S>(name) }
    {}

    virtual ~AbstractProduct() = default;

    virtual double get_calories() const = 0;
    virtual double get_proteins() const = 0;
    virtual double get_fats() const = 0;
    virtual double get_carbs() const = 0;

    std::string get_name() const
    {
        return name;
    }

    template<typename S>
    void set_name(S&& new_name)
    {
        name = std::forward<S>(new_name);
    }

private:
    std::string name;
};

#endif // ABSTRACTPRODUCT_H
