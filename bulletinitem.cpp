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
            double portion = j_m["weight"].get<double>() / 100.0;
            cal += j_m["calories"].get<double>() * portion;
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
    static const int capacity_threshold = 10;

    const int w = width();
    const int h = height();

    const int text_height { 10 };

    const int rect_width { w };
    int rect_height, pixels_left;
    if(goals.size() > capacity_threshold)
    {
        rect_height = h / goals.size();
        pixels_left = h - rect_height * goals.size();
    }
    else
    {
        rect_height = h / capacity_threshold;
        pixels_left = h - rect_height * capacity_threshold;
    }

    int rect_y { h - rect_height };
    QPainter painter(this);
    painter.setPen(Qt::white);
    QLinearGradient grad(0, 0, rect_width, 0);
    grad.setColorAt(1.0, Qt::white);
    for(const auto & color : goals)
    {
        grad.setColorAt(0.0, color);
        painter.setBrush(QBrush(grad));

        int height_stretch = (pixels_left--) > 0 ? 1 : 0;

        painter.drawRect(0, rect_y, rect_width, rect_height + height_stretch);
        rect_y -= (rect_height + height_stretch);
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    auto current_font = painter.font();
    current_font.setPointSize(9);
    painter.setFont(current_font);

    painter.drawText(0, 0.5 * text_height, w - 0.5 * text_height, text_height, Qt::AlignVCenter | Qt::AlignRight, QString::number(day));
    painter.drawText(0, h - 1.5 * text_height, w - 0.5 * text_height, text_height, Qt::AlignVCenter | Qt::AlignRight, QString::number(calories));
    painter.drawRect(0, 0, w - 1, h - 1);
}

