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
class QLineEdit;

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
    void table_updated();
    void calories_norm_changed(int norm);
    void proteins_norm_changed(int norm);
    void fats_norm_changed(int norm);
    void carbs_norm_changed(int norm);
    void reload_tables();

private:
    void switch_tables(int first_user_id, const QDate & first_date,
                       int second_user_id, const QDate & second_date);
    void pull_tables(int user_id, const QDate & date);
    void push_tables(int user_id, const QDate & date);

    void update_status(QLineEdit * le, int norm);

    Ui::MainWindow * ui;
    IngredientsDialog * ingredients_dialog;
    MealsDialog * meals_dialog;

    QStringList default_meals;
    ProductDictionary dict;
    std::vector<TableModel *> daily_user_tables;
    TableModel * current_model;
    QDate prev_date;
    int prev_user;
    int user_count;

    static const std::string user_data_path;
};

#endif // MAINWINDOW_H
