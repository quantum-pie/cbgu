#include "mealingredientsdialog.h"
#include "ui_mealingredientsdialog.h"
#include "tablemodel.h"
#include "meal.h"
#include "ingredientcompleterdelegate.h"
#include "productdictionary.h"

#include <QMenu>
#include <QAction>
#include <QMessageBox>

MealIngredientsDialog::MealIngredientsDialog(ProductDictionary & product_dict, const Meal * meal, QWidget * parent) :
    QDialog{ parent },
    ui{ new Ui::MealIngredientsDialog },
    product_dict_ref{ product_dict }
{
    ui->setupUi(this);
    table_model = new TableModel(product_dict, ui->tableView);
    ui->tableView->setModel(table_model);

    auto completer_delegate = new IngredientCompleterDelegate(product_dict, ui->tableView);
    ui->tableView->setItemDelegate(completer_delegate);

    if(meal)
    {
        for(auto ingredient : meal->get_ingredients())
        {
            table_model->insert_row(ingredient.first, 0);
        }
    }

    auto table_context_menu = new QMenu(ui->tableView);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto add_ingredient_action = new QAction("Add Ingredient", table_context_menu);
    auto remove_ingredient_action = new QAction("Remove Ingredient", table_context_menu);

    ui->tableView->addAction(add_ingredient_action);
    ui->tableView->addAction(remove_ingredient_action);

    connect(add_ingredient_action, SIGNAL(triggered()), this, SLOT(add_ingredient_triggered()));
    connect(remove_ingredient_action, SIGNAL(triggered()), this, SLOT(remove_ingredient_triggered()));
}

MealIngredientsDialog::~MealIngredientsDialog()
{
    delete ui;
}

std::string MealIngredientsDialog::get_ingredients() const
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
    name = ui->lineEdit->text().toStdString();
    if(!product_dict_ref.get(name))
    {
        for(int i = 0; i < table_model->rowCount(); ++i)
        {
            auto index = table_model->index(i, 0);
            auto ing_name = table_model->data(index).toString();
            index = table_model->index(i, 5);
            auto weight = table_model->data(index).toDouble();
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
        QMessageBox error_message;
        error_message.warning(0, "Error", "Product with such name already exists");
        error_message.setFixedSize(500, 200);
        error_message.show();
    }
}

void MealIngredientsDialog::cancel_pressed()
{
    reject();
}
