#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ingredientsdialog.h"
#include "mealingredientsdialog.h"
#include "meal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ingredients_dialog = new IngredientsDialog{ dict, this };

    Meal * new_meal = new Meal();

    meal_ingredients_dialog = new MealIngredientsDialog{ new_meal, dict, this };
    meal_ingredients_dialog->show();
    ingredients_dialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
