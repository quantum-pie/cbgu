#include "ingredientsdialog.h"
#include "ui_ingredientsdialog.h"

#include "treemodel.h"
#include "categorytreeitem.h"
#include "ingredienttreeitem.h"
#include "treeutils.h"
#include "productdictionary.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>

#include <fstream>
#include <iomanip>

using namespace nlohmann;

const std::string IngredientsDialog::tree_path { "res/lib/ingredients.tree" };

IngredientsDialog::IngredientsDialog(ProductDictionary & dict, QWidget *parent) :
    QMainWindow{ parent },
    ui{ new Ui::IngredientsDialog },
    search_meal_functor{ [](auto&){ return ""; } },
    product_dict_ref{ dict }
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(tr("Ingredients Library"));
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    json tree_backend;
    std::ifstream tree_desc{ tree_path };
    if(tree_desc.good())
    {
        tree_desc >> tree_backend;
    }

    tree_model = new TreeModel(ui->treeView);
    treeutils::build_tree(tree_model, product_dict_ref, tree_backend["value"]);

    ui->treeView->setModel(tree_model);
    for (int column = 0; column < tree_model->columnCount(); ++column)
        ui->treeView->resizeColumnToContents(column);

    auto tree_context_menu = new QMenu(ui->treeView);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto add_category_action = new QAction(QIcon(":/icons/icons/add.png"), tr("Add Category"), tree_context_menu);
    auto add_ingredient_action = new QAction(QIcon(":/icons/icons/add.png"), tr("Add Ingredient"), tree_context_menu);
    auto remove_item_action = new QAction(QIcon(":/icons/icons/garbage.png"), tr("Remove Item"), tree_context_menu);

    ui->treeView->addAction(add_category_action);
    ui->treeView->addAction(add_ingredient_action);
    ui->treeView->addAction(remove_item_action);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->header()->setStretchLastSection(true);

    connect(add_category_action, SIGNAL(triggered()), this, SLOT(add_category_triggered()));
    connect(add_ingredient_action, SIGNAL(triggered()), this, SLOT(add_ingredient_triggered()));
    connect(remove_item_action, SIGNAL(triggered()), this, SLOT(remove_item_triggered()));

    connect(ui->actionAdd_Root_Category, SIGNAL(triggered()), this, SLOT(add_root_category_triggered()));
    connect(ui->actionAdd_Root_Ingredient, SIGNAL(triggered()), this, SLOT(add_root_ingredient_triggered()));

    connect(ui->ok_button, SIGNAL(released()), this, SLOT(close()));
    connect(ui->ok_button, SIGNAL(released()), this, SIGNAL(closed()));

    connect(tree_model, SIGNAL(which_data_changed(const QVariant &,
                                                  const QModelIndex &)), this, SLOT(check_data_change(const QVariant &,
                                                                                                      const QModelIndex &)));
}

IngredientsDialog::~IngredientsDialog()
{
    std::ofstream o(tree_path);
    o << std::setw(4) << tree_model->get_json() << std::endl;
    delete ui;
}

void IngredientsDialog::add_category_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    auto main_index = index.sibling(index.row(), 0);
    if(tree_model->is_category(main_index))
    {
        add_category(main_index);
    }
}

void IngredientsDialog::add_ingredient_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    auto main_index = index.sibling(index.row(), 0);
    if(tree_model->is_category(main_index))
    {
        add_ingredient(main_index);
    }
}

void IngredientsDialog::remove_item_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    auto main_index = index.sibling(index.row(), 0);
    if(main_index.isValid())
    {
        bool to_delete { false };
        if(tree_model->is_category(main_index))
        {
            to_delete = !tree_model->hasChildren(main_index);
        }
        else
        {
            auto name { main_index.data().toString() };
            auto meal_name { search_meal_functor(name.toStdString()) };
            if(meal_name.empty())
            {
                if(treeutils::delete_question())
                {
                    to_delete = true;
                    product_dict_ref.remove(name);
                }
            }
            else
            {
                treeutils::is_used_error(QString::fromStdString(meal_name));
            }
        }

        if(to_delete)
        {
            tree_model->remove_row(main_index.row(), main_index.parent());
        }
    }
}

void IngredientsDialog::add_root_category_triggered()
{
    add_category();
}

void IngredientsDialog::add_root_ingredient_triggered()
{
    add_ingredient();
}

void IngredientsDialog::add_category(const QModelIndex & index)
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

void IngredientsDialog::add_ingredient(const QModelIndex & index)
{
    bool ok;
    auto text = QInputDialog::getText(this, tr("New ingredient"),
                                         tr("Please enter new ingredient name:"), QLineEdit::Normal,
                                         QString{}, &ok);

    if(ok)
    {
        auto std_text { text.toStdString() };
        if(std_text.empty())
        {
            treeutils::empty_name_error();
        }
        else if(!product_dict_ref.get(std_text))
        {
            auto new_ingredient = std::make_shared<Ingredient>(std_text);
            product_dict_ref.insert(new_ingredient);
            tree_model->insert_row(new IngredientTreeItem(std::move(new_ingredient)), 0, index);
        }
        else
        {
            treeutils::same_name_error();
        }
    }
}

void IngredientsDialog::check_data_change(const QVariant & before,
                       const QModelIndex & index)
{
    if(index.column() == 0 && !tree_model->is_category(index))
    {
        treeutils::dictionary_item_renamed(product_dict_ref, before.toString());
    }
}

