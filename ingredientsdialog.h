#ifndef INGREDIENTSDIALOG_H
#define INGREDIENTSDIALOG_H

#include "nlohmann/json.hpp"

#include <functional>

#include <QMainWindow>
#include <QModelIndex>

class TreeModel;
class Ingredient;
class ProductDictionary;

namespace Ui {
class IngredientsDialog;
}

class IngredientsDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit IngredientsDialog(ProductDictionary & dict, QWidget * parent = nullptr);
    ~IngredientsDialog();

    template<typename S>
    void set_searcher(S&& functor)
    {
        search_meal_functor = std::forward<S>(functor);
    }

private slots:
    void add_category_triggered();
    void add_ingredient_triggered();
    void remove_item_triggered();
    void add_root_category_triggered();
    void add_root_ingredient_triggered();
    void ok_pressed();
    void cancel_pressed();
    void check_data_change(const QVariant & before,
                           const QModelIndex & index);

private:
    void add_category(const QModelIndex & index = QModelIndex{});
    void add_ingredient(const QModelIndex & index = QModelIndex{});

    Ui::IngredientsDialog * ui;

    TreeModel * tree_model;

    std::function<std::string(const std::string&)> search_meal_functor;

    ProductDictionary & product_dict_ref;

    static const std::string tree_path;
};

#endif // INGREDIENTSDIALOG_H
