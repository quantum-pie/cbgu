#ifndef BULLETINCALENDAR_H
#define BULLETINCALENDAR_H

#include "bulletinitem.h"

#include <QCalendarWidget>

#include <string>

class BulletinCalendar : public QCalendarWidget
{
    Q_OBJECT

public:
    BulletinCalendar(const std::string & user_data_path, QWidget * parent = nullptr);

public slots:
    void user_changed(const QString & new_name);

private:
    virtual void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const override;

    std::string user_path_prefix;
    const std::string user_data_path;

    mutable BulletinItem item;
};

#endif // BULLETINCALENDAR_H
