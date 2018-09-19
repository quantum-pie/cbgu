#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QDate>

#include "tablemodel.h"
#include "productdictionary.h"
#include "abstractproduct.h"

namespace Ui {
class MainWindow;
}

class IngredientsDialog;
class MealsDialog;
class CheckableListModel;
class QLineEdit;
class BulletinCalendar;

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
    void add_temporary_triggered();
    void remove_product_triggered();
    void add_goal_triggered();
    void remove_goal_triggered();
    void table_updated();
    void calories_norm_changed(int norm);
    void proteins_norm_changed(int norm);
    void fats_norm_changed(int norm);
    void carbs_norm_changed(int norm);

private:
    void switch_tables(int first_user_id, const QDate & first_date,
                       int second_user_id, const QDate & second_date);
    void pull_tables(int user_id, const QDate & date);
    void push_tables(int user_id, const QDate & date);

    void update_status(QLineEdit * le, int norm);

    bool build_goals(const std::string & path);
    bool build_norm(const std::string & path);

    Ui::MainWindow * ui;
    IngredientsDialog * ingredients_dialog;
    MealsDialog * meals_dialog;

    QStringList default_meals;
    ProductDictionary dict;
    std::vector<TableModel *> daily_user_tables;
    TableModel aggregate_table;
    TableModel * current_model;
    CheckableListModel * daily_goals_list;
    BulletinCalendar * bulletin;

    QDate prev_date;
    int prev_user;
    int user_count;

    static const std::string user_data_path;
};

#endif // MAINWINDOW_H
