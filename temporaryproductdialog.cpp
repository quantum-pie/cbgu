#include "temporaryproductdialog.h"
#include "ui_temporaryproductdialog.h"

TemporaryProductDialog::TemporaryProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TemporaryProductDialog)
{
    ui->setupUi(this);
    connect(ui->ok_button, SIGNAL(released()), this, SLOT(ok_pressed()));
    connect(ui->cancel_button, SIGNAL(released()), this, SLOT(reject()));
}

TemporaryProductDialog::~TemporaryProductDialog()
{
    delete ui;
}

void TemporaryProductDialog::ok_pressed()
{
    name = ui->name_le->text().toStdString();
    params.calories = ui->calories_sb->value();
    params.proteins = ui->proteins_sb->value();
    params.fats = ui->fats_sb->value();
    params.carbs = ui->carbs_sb->value();
    accept();
}

std::pair<std::string, ProductParams>
TemporaryProductDialog::get_result() const
{
    return std::make_pair(name, params);
}
