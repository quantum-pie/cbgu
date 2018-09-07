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

const std::string MealsDialog::tree_path { "res/lib/meals.tree" };

MealsDialog::MealsDialog(ProductDictionary & dict, QWidget *parent) :
    QMainWindow{ parent },
    ui(new Ui::MealsDialog),
    product_dict_ref{ dict }
{
    ui->setupUi(this);

    setWindowTitle("Meals Library");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    json tree_backend;
    std::ifstream tree_desc{ tree_path };
    if(tree_desc.good())
    {
        tree_desc >> tree_backend;
    }

    tree_model = new TreeModel(ui->treeView);
    treeutils::build_tree(tree_model, product_dict_ref, tree_backend["value"]);

    connect(
        tree_model, &TreeModel::row_hard_removed,
        [this](const QModelIndex & index)
        {
            product_dict_ref.remove(index.data().toString());
        }
    );

    ui->treeView->setModel(tree_model);
    for (int column = 0; column < tree_model->columnCount(); ++column)
        ui->treeView->resizeColumnToContents(column);

    auto tree_context_menu = new QMenu(ui->treeView);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto add_category_action = new QAction("Add Category", tree_context_menu);
    auto add_meal_action = new QAction("Add Meal", tree_context_menu);
    auto edit_meal_action = new QAction("Edit Meal", tree_context_menu);
    auto remove_item_action = new QAction("Remove Item", tree_context_menu);

    ui->treeView->addAction(add_category_action);
    ui->treeView->addAction(add_meal_action);
    ui->treeView->addAction(edit_meal_action);
    ui->treeView->addAction(remove_item_action);

    connect(add_category_action, SIGNAL(triggered()), this, SLOT(add_category_triggered()));
    connect(add_meal_action, SIGNAL(triggered()), this, SLOT(add_meal_triggered()));
    connect(remove_item_action, SIGNAL(triggered()), this, SLOT(remove_item_triggered()));
    connect(edit_meal_action, SIGNAL(triggered()), this, SLOT(edit_meal_triggered()));

    connect(ui->actionAdd_Root_Category, SIGNAL(triggered()), this, SLOT(add_root_category_triggered()));
    connect(ui->actionAdd_Root_Meal, SIGNAL(triggered()), this, SLOT(add_root_meal_triggered()));

    connect(ui->ok_button, SIGNAL(released()), this, SLOT(ok_pressed()));
    connect(ui->cancel_button, SIGNAL(released()), this, SLOT(cancel_pressed()));

    connect(tree_model, SIGNAL(which_data_changed(const QVariant &,
                                                  const QModelIndex &)), this, SLOT(check_data_change(const QVariant &,
                                                                                                      const QModelIndex &)));
}

MealsDialog::~MealsDialog()
{
    std::ofstream o(tree_path);
    o << std::setw(4) << tree_model->get_json() << std::endl;
    delete ui;
}

std::string MealsDialog::is_used(const std::string & name, const QModelIndex & index) const
{
    if(tree_model->hasChildren(index))
    {
        std::string res;
        for(int i = 0; i < tree_model->rowCount(index); ++i)
        {
            res = is_used(name, tree_model->index(i, 0, index));
            if(!res.empty())
                return res;
        }
        return res;
    }
    else
    {
        auto meal_name { index.data().toString().toStdString() };
        auto item = static_cast<Meal *>(product_dict_ref.get(meal_name));
        if(item)
        {
            for(auto& ingredient : item->get_ingredients())
            {
                if(ingredient.first->get_name() == name)
                    return meal_name;
            }
        }
        return "";
    }
}

void MealsDialog::add_category_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    auto main_index = index.sibling(index.row(), 0);
    if(tree_model->is_category(main_index))
    {
        add_category(main_index);
    }
}

void MealsDialog::add_meal_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    auto main_index = index.sibling(index.row(), 0);
    if(tree_model->is_category(main_index))
    {
        add_meal(main_index);
    }
}

void MealsDialog::remove_item_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    auto main_index = index.sibling(index.row(), 0);
    if(main_index.isValid())
    {
        if(!tree_model->is_category(main_index))
            product_dict_ref.remove(main_index.data().toString());

        tree_model->weak_delete(main_index);
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
    auto main_index = index.sibling(index.row(), 0);
    if(main_index.isValid() && !tree_model->is_category(main_index))
    {
        auto name = main_index.data().toString().toStdString();
        auto meal = static_cast<Meal *>(product_dict_ref.get(name));
        MealIngredientsDialog meal_dlg{ product_dict_ref, meal };
        int result = meal_dlg.exec();
        if(result == QDialog::Accepted)
        {
            meal->set_name(meal_dlg.get_name());
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
        tree_model->weak_add(new CategoryTreeItem(text.toStdString()), index);
    }
}

void MealsDialog::add_meal(const QModelIndex & index)
{
    MealIngredientsDialog meal_dlg{ product_dict_ref };
    int result = meal_dlg.exec();
    if(result == QDialog::Accepted)
    {
        auto new_meal = new Meal(meal_dlg.get_name(), meal_dlg.get_ingredients());
        tree_model->weak_add(new MealTreeItem(new_meal), index);
        product_dict_ref.insert(new_meal);
    }
}

void MealsDialog::ok_pressed()
{
    tree_model->apply(true);
    close();
}

void MealsDialog::cancel_pressed()
{
    tree_model->apply(false);
    close();
}

void MealsDialog::check_data_change(const QVariant & before,
                       const QModelIndex & index)
{
    if(index.column() == 0 && !tree_model->is_category(index))
    {
        treeutils::dictionary_item_renamed(product_dict_ref, before.toString());
    }
}

