#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ingredientsdialog.h"
#include "mealsdialog.h"
#include "meal.h"
#include "tablemodel.h"
#include "ingredientcompleterdelegate.h"
#include "treeutils.h"

#include <QDirIterator>

#include <fstream>
#include <iomanip>

const std::string MainWindow::user_data_path { "res/usr/" };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{ parent },
    ui{ new Ui::MainWindow },
    prev_date{ QDate::currentDate() },
    prev_user{ 0 }
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMaximumDate(QDate::currentDate());

    auto completer_delegate = new IngredientCompleterDelegate(dict, ui->tableView);
    ui->tableView->setItemDelegate(completer_delegate);

    ingredients_dialog = new IngredientsDialog{ dict };
    meals_dialog = new MealsDialog{ dict };
    ingredients_dialog->set_searcher([this](auto& name) { return meals_dialog->is_used(name); } );

    default_meals << "Breakfast" << "Lunch" << "Dinner" << "Snack";

    QDirIterator it(QString::fromStdString(user_data_path), QDir::AllDirs | QDir::NoDotAndDotDot);
    while(it.hasNext())
    {
        ui->comboBox_user->addItem(it.next().split("/").last());
    }

    if(ui->comboBox_user->count())
    {
        ui->dateEdit->setEnabled(true);
        ui->comboBox_meal->setEnabled(true);
        push_tables(0, ui->dateEdit->date());
    }

    connect(ui->actionIngredients, SIGNAL(triggered(bool)), ingredients_dialog, SLOT(show()));
    connect(ui->actionMeals, SIGNAL(triggered(bool)), meals_dialog, SLOT(show()));

    connect(ui->dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(switch_date(QDate)));
    connect(ui->comboBox_user, SIGNAL(currentIndexChanged(int)), this, SLOT(switch_or_add_user(int)));
    connect(ui->comboBox_meal, SIGNAL(currentIndexChanged(int)), this, SLOT(switch_or_add_meal(int)));

    auto table_context_menu = new QMenu(ui->tableView);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);

    auto add_ingredient_action = new QAction("Add Product", table_context_menu);
    auto remove_ingredient_action = new QAction("Remove Product", table_context_menu);

    ui->tableView->addAction(add_ingredient_action);
    ui->tableView->addAction(remove_ingredient_action);

    connect(add_ingredient_action, SIGNAL(triggered()), this, SLOT(add_product_triggered()));
    connect(remove_ingredient_action, SIGNAL(triggered()), this, SLOT(remove_product_triggered()));
}

MainWindow::~MainWindow()
{
    pull_tables(ui->comboBox_user->currentIndex(), ui->dateEdit->date());

    delete ui;
    delete meals_dialog;
    delete ingredients_dialog;
}

void MainWindow::switch_or_add_user(int user_id)
{
    ui->dateEdit->setEnabled(true);
    ui->comboBox_meal->setEnabled(true);
    if(user_id == static_cast<int>(ui->comboBox_user->count()))
    {
        QDir user_data_dir { QString::fromStdString(user_data_path) };
        auto new_user_name = ui->comboBox_user->currentText();
        user_data_dir.mkdir(new_user_name);
    }

    auto current_date = ui->dateEdit->date();
    switch_tables(prev_user, current_date, user_id, current_date);
    prev_user = user_id;
}

void MainWindow::switch_or_add_meal(int meal_id)
{
    if(meal_id == static_cast<int>(daily_user_tables.size()))
    {
        daily_user_tables.emplace_back(new TableModel(dict));
    }

    current_model = daily_user_tables[meal_id];
    ui->tableView->setModel(current_model);
}

void MainWindow::switch_date(const QDate & new_date)
{
    auto current_user_id = ui->comboBox_user->currentIndex();
    switch_tables(current_user_id, prev_date, current_user_id, new_date);
    prev_date = new_date;
}

void MainWindow::add_product_triggered()
{
    current_model->create_row(0);
}

void MainWindow::remove_product_triggered()
{
    auto index = ui->tableView->selectionModel()->currentIndex();
    if(index.isValid())
    {
        current_model->remove_row(index.row());
    }
}

void MainWindow::pull_tables(std::size_t user_id, const QDate & date)
{
    ui->comboBox_meal->blockSignals(true);

    auto user_name = ui->comboBox_user->itemText(user_id).toStdString();
    std::string path { user_data_path + user_name + '/' + treeutils::date_to_string(date) + ".dat" };
    std::ofstream o{ path };
    auto j = json::array();
    for(std::size_t table_id = 0; table_id < daily_user_tables.size(); ++table_id)
    {
        json j_table;
        j_table["name"] = ui->comboBox_meal->itemText(table_id).toStdString();
        j_table["value"] = daily_user_tables[table_id]->get_json();
        j.emplace_back(j_table);
        delete daily_user_tables[table_id];
    }
    o << std::setw(4) << j;

    ui->comboBox_meal->clear();
    ui->comboBox_meal->blockSignals(false);
}

void MainWindow::push_tables(std::size_t user_id, const QDate & date)
{
    ui->comboBox_meal->blockSignals(true);

    auto user_name = ui->comboBox_user->itemText(user_id).toStdString();
    std::string path = { user_data_path + user_name + '/' + treeutils::date_to_string(date) + ".dat" };
    std::ifstream in{ path };
    auto j = json::array();
    std::size_t new_table_size;
    if(in.good())
    {
        in >> j;
        new_table_size = j.size();
    }
    else
    {
        new_table_size = default_meals.size();
        ui->comboBox_meal->addItems(default_meals);
    }

    daily_user_tables.resize(new_table_size);
    std::generate_n(daily_user_tables.begin(), new_table_size,
        [this](){ return new TableModel(dict); });

    if(in.good())
    {
        auto rebuild_functor = [this](TableModel * table, const json & js)
        {
            return treeutils::build_table(table, dict, js["value"]);
        };

        std::transform(daily_user_tables.begin(), daily_user_tables.end(),
                       j.begin(), daily_user_tables.begin(), std::move(rebuild_functor));

        std::for_each(j.begin(), j.end(),
        [this](auto & j_it)
        {
            ui->comboBox_meal->addItem(QString::fromStdString(j_it["name"]));
        });
    }

    ui->comboBox_meal->blockSignals(false);
    switch_or_add_meal(ui->comboBox_meal->currentIndex());
}

void MainWindow::switch_tables(std::size_t first_user_id, const QDate & first_date,
                   std::size_t second_user_id, const QDate & second_date)
{
    pull_tables(first_user_id, first_date);
    push_tables(second_user_id, second_date);
}


