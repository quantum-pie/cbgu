#include "bulletincalendar.h"
#include "treeutils.h"

#include <QPainter>

#include <fstream>

BulletinCalendar::BulletinCalendar(const std::string & user_data_path, QWidget * parent)
    : QCalendarWidget{ parent }, user_data_path { user_data_path }
{
    setMaximumDate(QDate::currentDate());
    setUpdatesEnabled(true);
    setEnabled(true);
}

void BulletinCalendar::user_changed(const QString & new_name)
{
    user_path_prefix = user_data_path + new_name.toStdString() + '/';

    // force repaint
    setCurrentPage(yearShown() + 1, monthShown());
    setCurrentPage(yearShown() - 1, monthShown());
}

void BulletinCalendar::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    auto date_string = treeutils::date_to_string(date);
    auto goals_path = user_path_prefix + date_string + ".goals";
    auto energy_path = user_path_prefix + date_string + ".energy";

    std::ifstream in_goals(goals_path);
    std::ifstream in_energy(energy_path);

    bool data_ok { in_goals.good() && in_energy.good() };
    if(data_ok)
    {
        json j_goals;
        in_goals >> j_goals;
        item.set_goals(j_goals);

        auto j_energy = json::array();
        in_energy >> j_energy;
        item.set_calories(j_energy);

        item.set_day(date.day());

        painter->save();
        item.resize(rect.size());
        painter->translate(rect.topLeft());
        item.render(painter, QPoint(), QRegion(), QWidget::DrawChildren );
        painter->restore();
    }
    else QCalendarWidget::paintCell(painter, rect, date);
}
