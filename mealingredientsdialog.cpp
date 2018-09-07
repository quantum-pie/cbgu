#include "mealingredientsdialog.h"
#include "ui_mealingredientsdialog.h"
#include "tablemodel.h"
#include "meal.h"
#include "ingredientcompleterdelegate.h"
#include "productdictionary.h"
#include "treeutils.h"

#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QPushButton>

MealIngredientsDialog::MealIngredientsDialog(ProductDictionary & product_dict, const Meal * meal, QWidget * parent) :
    QDialog{ parent },
    ui{ new Ui::MealIngredientsDialog },
    product_dict_ref{ product_dict }
{
    ui->setupUi(this);

    setWindowTitle("Edit Meal");

    table_model = new TableModel(product_dict, ui->tableView);
    ui->tableView->setModel(table_model);

    auto completer_delegate = new IngredientCompleterDelegate(product_dict, ui->tableView);
    ui->tableView->setItemDelegate(completer_delegate);

    if(meal)
    {
        ui->lineEdit->setText(QString::fromStdString(meal->get_name()));
        name = meal->get_name();
        for(auto& ingredient : meal->get_ingredients())
        {
            table_model->insert_row(ingredient.first, 0);
            table_model->setData(table_model->index(0, table_model->weight_idx()), ingredient.second);
        }
    }

    auto table_context_menu = new QMenu(ui->tableView);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto add_ingredient_action = new QAction("Add Ingredient", table_context_menu);
    auto remove_ingredient_action = new QAction("Remove Ingredient", table_context_menu);

    ui->tableView->addAction(add_ingredient_action);
    ui->tableView->addAction(remove_ingredient_action);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(add_ingredient_action, SIGNAL(triggered()), this, SLOT(add_ingredient_triggered()));
    connect(remove_ingredient_action, SIGNAL(triggered()), this, SLOT(remove_ingredient_triggered()));

    connect(ui->ok_button, SIGNAL(released()), this, SLOT(ok_pressed()));
    connect(ui->cancel_button, SIGNAL(released()), this, SLOT(cancel_pressed()));
}

MealIngredientsDialog::~MealIngredientsDialog()
{
    delete ui;
}

Meal::ingredients_container_type MealIngredientsDialog::get_ingredients() const
{
    return ingredients;
}

std::string MealIngredientsDialog::get_name() const
{
    return name;
}

void MealIngredientsDialog::add_ingredient_triggered()
{
    table_model->create_row(0);
}

void MealIngredientsDialog::remove_ingredient_triggered()
{
    auto index = ui->tableView->selectionModel()->currentIndex();
    if(index.isValid())
    {
        table_model->remove_row(index.row());
    }
}

void MealIngredientsDialog::ok_pressed()
{
    auto new_name = ui->lineEdit->text().toStdString();
    if(new_name.empty())
    {
        treeutils::empty_name_error();
    }
    else if(table_model->rowCount() == 0)
    {
        treeutils::empty_composition_error();
    }
    else if(name == new_name || !product_dict_ref.get(new_name))
    {
        name = new_name;
        for(int i = 0; i < table_model->rowCount(); ++i)
        {
            auto index = table_model->index(i, table_model->name_idx());
            auto ing_name = index.data().toString();
            index = table_model->index(i, table_model->weight_idx());
            auto weight = index.data().toDouble();
            auto item = product_dict_ref.get(ing_name.toStdString());
            if(item)
            {
                ingredients.emplace_back(item, weight);
            }
        }
        accept();
    }
    else
    {
        treeutils::same_name_error();
    }
}

void MealIngredientsDialog::cancel_pressed()
{
    reject();
}
