#include "bulletinitem.h"
#include "ui_bulletinitem.h"
#include "checkablelistmodel.h"
#include "treeutils.h"

#include <QPainter>
#include <QLinearGradient>

#include <cmath>

BulletinItem::BulletinItem(QWidget * parent) :
    QWidget{ parent },
    ui{ new Ui::BulletinItem }
{
    ui->setupUi(this);
}

BulletinItem::~BulletinItem()
{
    delete ui;
}

void BulletinItem::set_goals(const json& j)
{
    goals.clear();
    if(!j.is_null())
    {
        for(auto it = j.begin(); it != j.end(); ++it)
        {
            auto val = it.value();
            if(val["checked"])
            {
                goals.emplace_back(QString::fromStdString(val["color"]));
            }
        }
    }
}

void BulletinItem::set_calories(const json& j)
{
    double cal {0};
    for(auto & j_el : j)
    {
        for(auto & j_m : j_el["value"])
        {
            cal += j_m["calories"].get<double>();
        }
    }
    calories = static_cast<int>(cal);
}

void BulletinItem::set_day(int new_day)
{
    day = new_day;
}

void BulletinItem::paintEvent(QPaintEvent *)
{
    const int w = width();
    const int h = height();
    const double dh = h;

    const int rect_width { w };
    int rect_height;
    if(goals.size() < 10)
        rect_height = std::round(dh / 10);
    else
        rect_height = std::round(dh / goals.size());

    static const int text_height { 20 };

    int rect_y { h - rect_height };
    QPainter painter(this);
    painter.setPen(Qt::white);
    QLinearGradient grad(0, 0, rect_width, 0);
    grad.setColorAt(1.0, Qt::white);
    for(const auto & color : goals)
    {
        grad.setColorAt(0.0, color);
        painter.setBrush(QBrush(grad));
        painter.drawRect(0, rect_y, rect_width, rect_height);
        rect_y -= rect_height;
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);

    painter.drawText(0, 0, w - text_height, text_height, Qt::AlignVCenter | Qt::AlignRight, QString::number(day));
    painter.drawText(0, h - text_height, w - text_height, text_height, Qt::AlignVCenter | Qt::AlignRight, QString::number(calories));
    painter.drawRect(0, 0, w - 1, h - 1);
}

