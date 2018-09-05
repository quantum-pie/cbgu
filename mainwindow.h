#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QDate>

#include "productdictionary.h"

namespace Ui {
class MainWindow;
}

class IngredientsDialog;
class MealsDialog;
class TableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

private slots:
    void switch_or_add_user(int user_id);
    void switch_or_add_meal(int meal_id);
    void switch_date(const QDate & new_date);
    void add_product_triggered();
    void remove_product_triggered();

private:
    Ui::MainWindow * ui;
    IngredientsDialog * ingredients_dialog;
    MealsDialog * meals_dialog;

    QStringList default_meals;
    ProductDictionary dict;
    std::vector<std::vector<TableModel *>> daily_user_tables;
    TableModel * current_model;
    QDate prev_date;

    static const std::string user_data_path;
};

#endif // MAINWINDOW_H
