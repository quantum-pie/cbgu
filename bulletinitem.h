#ifndef BULLETINITEM_H
#define BULLETINITEM_H

#include "nlohmann/json.hpp"

#include <QWidget>

using namespace nlohmann;

class CheckableListModel;

namespace Ui {
class BulletinItem;
}

class BulletinItem : public QWidget
{
    Q_OBJECT

public:
    explicit BulletinItem(QWidget * parent = nullptr);
    ~BulletinItem();

    void set_goals(const json& j);
    void set_calories(const json& j);
    void set_day(int day);

private:
    Ui::BulletinItem * ui;
    CheckableListModel * daily_goals_list;
};

#endif // BULLETINITEM_H
