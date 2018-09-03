#ifndef INGREDIENTSDIALOG_H
#define INGREDIENTSDIALOG_H

#include "nlohmann/json.hpp"

#include <QMainWindow>
#include <QModelIndex>

#include <memory>

class TreeModel;
class Ingredient;

namespace Ui {
class IngredientsDialog;
}

class IngredientsDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit IngredientsDialog(QWidget * parent = nullptr);
    ~IngredientsDialog();

private slots:
    void add_category_triggered();
    void add_ingredient_triggered();
    void remove_item_triggered();
    void add_root_category_triggered();
    void add_root_ingredient_triggered();
    void ok_pressed();
    void cancel_pressed();

private:
    void save_tree();

    void add_category(const QModelIndex & index = QModelIndex{});
    void add_ingredient(const QModelIndex & index = QModelIndex{});

    Ui::IngredientsDialog * ui;

    TreeModel * tree_model;
    nlohmann::json tree_backend;

    static const std::string tree_path;
};

#endif // INGREDIENTSDIALOG_H
