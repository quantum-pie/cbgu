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

    std::string is_used(const std::string & name, const QModelIndex & index = QModelIndex{}) const;

private slots:
    void add_category_triggered();
    void add_meal_triggered();
    void remove_item_triggered();
    void add_root_category_triggered();
    void add_root_meal_triggered();
    void edit_meal_triggered();
    void check_data_change(const QVariant & before,
                           const QModelIndex & index);

private:
    void add_category(const QModelIndex & index = QModelIndex{});
    void add_meal(const QModelIndex & index = QModelIndex{});
    void edit_meal(Meal * meal);

    Ui::MealsDialog *ui;

    TreeModel * tree_model;

    ProductDictionary & product_dict_ref;

    static const std::string tree_path;
};

#endif // MEALSDIALOG_H
