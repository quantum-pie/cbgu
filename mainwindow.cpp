#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ingredient.h"
#include "meal.h"

#include <iostream>
#include <memory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto milk = std::make_shared<Ingredient>( "milk", 1, 2, 3, 4 );
    auto bread = std::make_shared<Ingredient>( "bread", 5, 7, 5, 3 );

    std::vector<std::shared_ptr<const AbstractProduct>> basket{ milk, bread };
    Meal milk_with_bread{ "milk_with_bread", std::move(basket) };

    std::cout  << milk_with_bread.get_name() << std::endl
               << milk_with_bread.get_calories() << std::endl
               << milk_with_bread.get_proteins() << std::endl
               << milk_with_bread.get_fats() << std::endl
               << milk_with_bread.get_carbs() << std::endl;

    for(auto ingredient : milk_with_bread.get_ingredients())
    {
        std::cout << ingredient->get_name() << std::endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
