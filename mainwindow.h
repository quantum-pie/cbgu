#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "productdictionary.h"

namespace Ui {
class MainWindow;
}

class IngredientsDialog;
class MealIngredientsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    IngredientsDialog *ingredients_dialog;
    MealIngredientsDialog *meal_ingredients_dialog;
    ProductDictionary dict;
};

#endif // MAINWINDOW_H
