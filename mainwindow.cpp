#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ingredientsdialog.h"
#include "mealsdialog.h"
#include "meal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ingredients_dialog = new IngredientsDialog{ dict, this };
    meals_dialog = new MealsDialog{ dict, this };

    ingredients_dialog->show();
    meals_dialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
