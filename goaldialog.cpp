#include "goaldialog.h"
#include "ui_goaldialog.h"

#include <QPushButton>

GoalDialog::GoalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoalDialog)
{
    ui->setupUi(this);

    connect(ui->ok_button, SIGNAL(released()), this, SLOT(ok_pushed()));
    connect(ui->cancel_button, SIGNAL(released()), this, SLOT(reject()));
    connect(ui->color_button, SIGNAL(released()), &color_dialog, SLOT(exec()));
    connect(&color_dialog, SIGNAL(accepted()), this, SLOT(color_chosen()));

    setWindowIcon(QIcon(":/icons/icons/success.png"));
}

GoalDialog::~GoalDialog()
{
    delete ui;
}

void GoalDialog::ok_pushed()
{
    data.first = ui->name_le->text().toStdString();
    accept();
}

void GoalDialog::color_chosen()
{
    data.second = color_dialog.selectedColor();
    QPalette palette;
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Base, data.second);
    ui->name_le->setPalette(palette);
}

std::pair<std::string, QColor> GoalDialog::get_result() const
{
    return data;
}
