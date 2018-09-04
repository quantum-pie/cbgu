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

const std::string IngredientsDialog::tree_path { "res/ingredients.tree" };

IngredientsDialog::IngredientsDialog(ProductDictionary & dict, QWidget *parent) :
    QMainWindow{ parent },
    ui(new Ui::IngredientsDialog),
    product_dict_ref{ dict }
{
    ui->setupUi(this);

    setWindowTitle("Ingredients Library");
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
    auto add_ingredient_action = new QAction("Add Ingredient", tree_context_menu);
    auto remove_item_action = new QAction("Remove Item", tree_context_menu);

    ui->treeView->addAction(add_category_action);
    ui->treeView->addAction(add_ingredient_action);
    ui->treeView->addAction(remove_item_action);

    connect(add_category_action, SIGNAL(triggered()), this, SLOT(add_category_triggered()));
    connect(add_ingredient_action, SIGNAL(triggered()), this, SLOT(add_ingredient_triggered()));
    connect(remove_item_action, SIGNAL(triggered()), this, SLOT(remove_item_triggered()));

    connect(ui->actionAdd_Root_Category, SIGNAL(triggered()), this, SLOT(add_root_category_triggered()));
    connect(ui->actionAdd_Root_Ingredient, SIGNAL(triggered()), this, SLOT(add_root_ingredient_triggered()));

    connect(ui->ok_button, SIGNAL(released()), this, SLOT(ok_pressed()));
    connect(ui->cancel_button, SIGNAL(released()), this, SLOT(cancel_pressed()));
}

void IngredientsDialog::add_category_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    if(tree_model->is_category(index))
    {
        add_category(index);
    }
}

void IngredientsDialog::add_ingredient_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    if(tree_model->is_category(index))
    {
        add_ingredient(index);
    }
}

void IngredientsDialog::remove_item_triggered()
{
    auto index = ui->treeView->selectionModel()->currentIndex();
    if(index.isValid())
    {
        if(!tree_model->is_category(index))
            product_dict_ref.remove(index.data().toString());

        tree_model->remove_row(index.row(), index.parent());
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

    if(ok && !text.isEmpty())
    {
        auto std_text { text.toStdString() };
        if(!product_dict_ref.get(std_text))
        {
            auto new_ingredient = new Ingredient(std_text);
            tree_model->insert_row(new IngredientTreeItem(new_ingredient), 0, index);
            product_dict_ref.insert(new_ingredient);
        }
        else
        {
            QMessageBox error_message;
            error_message.warning(0, "Error", "Product with such name already exists");
            error_message.setFixedSize(500, 200);
            error_message.show();
        }
    }
}

void IngredientsDialog::ok_pressed()
{
    tree_backend = tree_model->get_json();
    close();
}

void IngredientsDialog::cancel_pressed()
{
    treeutils::rebuild_tree(tree_model, product_dict_ref, tree_backend["value"]);
    close();
}

IngredientsDialog::~IngredientsDialog()
{
    std::ofstream o(tree_path);
    o << std::setw(4) << tree_backend << std::endl;
    delete ui;
}
