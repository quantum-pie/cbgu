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
    prev_date{ QDate::currentDate() }
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMaximumDate(QDate::currentDate());

    auto completer_delegate = new IngredientCompleterDelegate(dict, ui->tableView);
    ui->tableView->setItemDelegate(completer_delegate);

    ingredients_dialog = new IngredientsDialog{ dict };
    meals_dialog = new MealsDialog{ dict };
    ingredients_dialog->set_searcher([this](auto& name) { return meals_dialog->is_used(name); } );

    for(int i = 0; i < ui->comboBox_meal->count(); ++i)
    {
        default_meals << ui->comboBox_meal->itemText(i);
    }

    std::size_t users_count {0};
    QDirIterator it(QString::fromStdString(user_data_path), QDir::AllDirs | QDir::NoDotAndDotDot);
    while(it.hasNext())
    {
        users_count++;
        ui->comboBox_user->addItem(it.next().split("/").last());
    }

    auto tables_per_user = static_cast<std::size_t>(ui->comboBox_meal->count());
    if(users_count)
    {
        daily_user_tables.resize(users_count);
        std::for_each(daily_user_tables.begin(), daily_user_tables.end(),
                        [this, tables_per_user](auto& tables)
                        {
                            tables.resize(tables_per_user);
                            std::generate_n(tables.begin(), tables_per_user,
                                   [this](){ return new TableModel(dict, ui->tableView); });
                        });

        switch_or_add_user(0);
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
    delete ui;
    delete meals_dialog;
    delete ingredients_dialog;
}

void MainWindow::switch_or_add_user(int user_id)
{
    ui->dateEdit->setEnabled(true);
    ui->comboBox_meal->setEnabled(true);
    if(user_id == static_cast<int>(daily_user_tables.size()))
    {
        auto tables_per_user = static_cast<std::size_t>(ui->comboBox_meal->count());
        daily_user_tables.emplace_back();
        daily_user_tables[user_id].resize(tables_per_user);

        std::generate_n(daily_user_tables[user_id].begin(), tables_per_user,
            [this](){ return new TableModel(dict, ui->tableView); });

        QDir user_data_dir { QString::fromStdString(user_data_path) };
        auto new_user_name = ui->comboBox_user->currentText();
        user_data_dir.mkdir(new_user_name);
    }

    current_model = daily_user_tables[user_id][ui->comboBox_meal->currentIndex()];
    ui->tableView->setModel(current_model);
}

void MainWindow::switch_or_add_meal(int meal_id)
{
    int current_user_id { ui->comboBox_user->currentIndex() };
    if(meal_id == static_cast<int>(daily_user_tables[current_user_id].size()))
    {
        daily_user_tables[current_user_id].emplace_back(new TableModel(dict, ui->tableView));
    }

    current_model = daily_user_tables[current_user_id][meal_id];
    ui->tableView->setModel(current_model);
}

void MainWindow::switch_date(const QDate & new_date)
{
    if(new_date != prev_date)
    {
        auto prev_date_string = std::to_string(prev_date.day()) + '_' +
                               std::to_string(prev_date.month()) + '_' +
                               std::to_string(prev_date.year());

        auto new_date_string = std::to_string(new_date.day()) + '_' +
                               std::to_string(new_date.month()) + '_' +
                               std::to_string(new_date.year());

        for(std::size_t user_id = 0; user_id < daily_user_tables.size(); ++user_id)
        {
            auto user_name = ui->comboBox_user->itemText(user_id).toStdString();
            std::string path { user_data_path + user_name + '/' + prev_date_string + ".dat" };
            std::ofstream o{ path };
            auto j = json::array();
            for(std::size_t table_id = 0; table_id < daily_user_tables[user_id].size(); ++table_id)
            {
                json j_table;
                j_table[ui->comboBox_meal->itemText(table_id).toStdString()] = daily_user_tables[user_id][table_id]->get_json();
                j.emplace_back(j_table);
                delete daily_user_tables[user_id][table_id];
            }
            o << std::setw(4) << j;
            o.close();

            // TODO rebuild table models from file for user
            path = { user_data_path + user_name + '/' + new_date_string + ".dat" };
            std::ifstream in{ path };
            j.clear();
            std::size_t new_table_size;
            ui->comboBox_meal->clear();
            if(in.good())
            {
                in >> j;
                new_table_size = j.size();

                int idx {0};
                for(auto it = j.begin(); it != j.end(); ++it)
                {
                    ui->comboBox_meal->insertItem(idx++, QString::fromStdString(it.key()));
                }
            }
            else
            {
                new_table_size = default_meals.size();
                ui->comboBox_meal->insertItems(0, default_meals);
            }

            daily_user_tables[user_id].resize(new_table_size);
            for(std::size_t table_id = 0; table_id < daily_user_tables[user_id].size(); ++table_id)
            {
                daily_user_tables[user_id][table_id] = new TableModel(dict, ui->tableView);
                if(in.good())
                {
                    treeutils::rebuild_table(daily_user_tables[user_id][table_id], j[table_id]);
                }
            }
        }

        prev_date = new_date;
        switch_or_add_user(ui->comboBox_user->currentIndex());
    }
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

