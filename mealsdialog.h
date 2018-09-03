#ifndef MEALSDIALOG_H
#define MEALSDIALOG_H

#include "nlohmann/json.hpp"

#include <QMainWindow>
#include <QModelIndex>

#include <QMainWindow>

class TreeModel;
class Meal;
class ProductDictionary;

namespace Ui {
class MealsDialog;
}

class MealsDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit MealsDialog(ProductDictionary & dict, QWidget *parent = nullptr);
    ~MealsDialog();

private slots:
    void add_category_triggered();
    void add_meal_triggered();
    void remove_item_triggered();
    void add_root_category_triggered();
    void add_root_meal_triggered();
    void ok_pressed();
    void cancel_pressed();

private:
    void add_category(const QModelIndex & index = QModelIndex{});
    void add_meal(const QModelIndex & index = QModelIndex{});

    Ui::MealsDialog *ui;

    TreeModel * tree_model;
    nlohmann::json tree_backend;

    ProductDictionary & product_dict_ref;

    static const std::string tree_path;
};

#endif // MEALSDIALOG_H