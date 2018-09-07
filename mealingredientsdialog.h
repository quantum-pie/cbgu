#ifndef MEALINGREDIENTSDIALOG_H
#define MEALINGREDIENTSDIALOG_H

#include "meal.h"

#include <QDialog>

class TableModel;
class ProductDictionary;

namespace Ui {
class MealIngredientsDialog;
}

class MealIngredientsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MealIngredientsDialog(ProductDictionary & product_dict, std::shared_ptr<const Meal> meal = nullptr, QWidget * parent = nullptr);
    ~MealIngredientsDialog();

    Meal::ingredients_container_type get_ingredients() const;
    std::string get_name() const;

private slots:
    void add_ingredient_triggered();
    void remove_ingredient_triggered();
    void ok_pressed();
    void cancel_pressed();

private:
    Ui::MealIngredientsDialog * ui;

    TableModel * table_model;

    ProductDictionary & product_dict_ref;

    Meal::ingredients_container_type ingredients;
    std::string name;
};

#endif // MEALINGREDIENTSDIALOG_H
