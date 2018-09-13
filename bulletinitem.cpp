#include "bulletinitem.h"
#include "ui_bulletinitem.h"
#include "checkablelistmodel.h"
#include "treeutils.h"

BulletinItem::BulletinItem(QWidget * parent) :
    QWidget{ parent },
    ui{ new Ui::BulletinItem }
{
    ui->setupUi(this);

    daily_goals_list = new CheckableListModel(ui->listView);
    ui->listView->setModel(daily_goals_list);
}

void BulletinItem::set_goals(const json& j)
{
    treeutils::build_optimist_list(daily_goals_list, j);
}

void BulletinItem::set_calories(const json& j)
{
    double calories {0};
    for(auto & j_el : j)
    {
        for(auto & j_m : j_el["value"])
        {
            calories += j_m["calories"].get<double>();
        }
    }
    ui->calories_le->setText(QString::number(static_cast<int>(calories)));
}

void BulletinItem::set_day(int day)
{
    ui->day_le->setText(QString::number(day));
}

BulletinItem::~BulletinItem()
{
    delete ui;
}
