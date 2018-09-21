#include "goaldialog.h"
#include "ui_goaldialog.h"

#include <QPushButton>

GoalDialog::GoalDialog(const item_type & old_item, QWidget *parent)
    : QDialog{ parent },
      ui{ new Ui::GoalDialog }
{
    ui->setupUi(this);

    ui->name_le->setText(QString::fromStdString(old_item.first));
    set_background(old_item.second);

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
    set_background(data.second);
}

std::pair<std::string, QColor> GoalDialog::get_result() const
{
    return data;
}

void GoalDialog::set_background(const QColor & color)
{
    QPalette palette;
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Base, color);
    ui->name_le->setPalette(palette);
}
