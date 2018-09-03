#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ingredientsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ingredients_dialog = new IngredientsDialog{ dict, this };
    ingredients_dialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
