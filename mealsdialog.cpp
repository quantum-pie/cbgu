#include "mealsdialog.h"
#include "ui_mealsdialog.h"

#include "treemodel.h"
#include "categorytreeitem.h"
#include "mealtreeitem.h"
#include "treeutils.h"
#include "productdictionary.h"
#include "mealingredientsdialog.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>

#include <fstream>
#include <iomanip>

using namespace nlohmann;

const std::string MealsDialog::tree_path { "res/meals.tree" };

MealsDialog::MealsDialog(ProductDictionary & dict, QWidget *parent) :
    QMainWindow{ parent },
    ui(new Ui::MealsDialog),
    product_dict_ref{ dict }
{
    ui->setupUi(this);

    setWindowTitle("Meals Library");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    std::ifstream tree_desc{ tree_path };
    if(tree_desc.good())
    {
        tree_desc >> tree_backend;
    }

    tree_model = new TreeModel(ui->treeView);
    treeutils::rebuild_tree(tree_model, product_dict_ref, tree_backend["value"]);

    ui->treeView->setModel(tree_model);
    for (int column = 0; column < tree_model->columnCount(); ++column)
        ui->treeView->resizeColumnToContents(column);

    auto tree_context_menu = new QMenu(ui->treeView);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto add_category_action = new QAction("Add Category", tree_context_menu);
    auto add_meal_action = new QAction("Add Meal", tree_context_menu);
    auto remove_item_action = new QAction("Remove Item", tree_context_menu);

    ui->treeView->addAction(add_category_action);
    ui->treeView->addAction(add_meal_action);
    ui->treeView->addAction(remove_item_action);

    connect(add_category_action, SIGNAL(triggered()), this, SLOT(add_category_triggered()));
    connect(add_meal_action, SIGNAL(triggered()), this, SLOT(add_meal_triggered()));
    connect(remove_item_action, SIGNAL(triggered()), this, SLOT(remove_item_triggered()));

    connect(ui->actionAdd_Root_Category, SIGNAL(triggered()), this, SLOT(add_root_category_triggered()));
    connect(ui->actionAdd_Root_Meal, SIGNAL(triggered()), this, SLOT(add_root_meal_triggered()));

    connect(ui->ok_button, SIGNAL(released()), this, SLOT(ok_pressed()));
    connect(ui->cancel_button, SIGNAL(released()), this, SLOT(cancel_pressed()));
}

void MealsDialog::add_category_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    if(tree_model->is_category(index))
    {
        add_category(index);
    }
}

void MealsDialog::add_meal_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    if(tree_model->is_category(index))
    {
        add_meal(index);
    }
}

void MealsDialog::remove_item_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    if(index.isValid())
    {
        tree_model->remove_row(index.row(), index.parent());
    }
}

void MealsDialog::add_root_category_triggered()
{
    add_category();
}

void MealsDialog::add_root_meal_triggered()
{
    add_meal();
}

void MealsDialog::edit_meal_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    if(index.isValid())
    {
        // TODO bad encapsulation
         auto meal = static_cast<Meal *>(tree_model->get_item(index));
         MealIngredientsDialog meal_dlg{ product_dict_ref, meal };
         int result = meal_dlg.exec();
         if(result == QDialog::Accepted)
         {
            meal->set_ingredients(meal_dlg.get_ingredients());
         }
    }
}

void MealsDialog::add_category(const QModelIndex & index)
{
    bool ok;
    auto text = QInputDialog::getText(this, tr("New category"),
                                         tr("Please enter new category name:"), QLineEdit::Normal,
                                         QString{}, &ok);
    if(ok && !text.isEmpty())
    {
        tree_model->insert_row(new CategoryTreeItem(text.toStdString()), 0, index);
    }
}

void MealsDialog::add_meal(const QModelIndex & index)
{
    MealIngredientsDialog meal_dlg{ product_dict_ref };
    int result = meal_dlg.exec();
    if(result == QDialog::Accepted)
    {
        auto new_meal = new Meal(meal_dlg.get_name(), meal_dlg.get_ingredients());
        tree_model->insert_row(new_meal, 0, index);
    }
}

void MealsDialog::ok_pressed()
{
    tree_backend = tree_model->get_json();
    close();
}

void MealsDialog::cancel_pressed()
{
    treeutils::rebuild_tree(tree_model, product_dict_ref, tree_backend["value"]);
    close();
}

MealsDialog::~MealsDialog()
{
    std::ofstream o(tree_path);
    o << std::setw(4) << tree_backend << std::endl;
    delete ui;
}
