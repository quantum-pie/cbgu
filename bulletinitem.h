#ifndef BULLETINITEM_H
#define BULLETINITEM_H

#include "nlohmann/json.hpp"

#include <QWidget>

#include <vector>

using namespace nlohmann;

namespace Ui {
class BulletinItem;
}

class QPaintEvent;

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
    void paintEvent(QPaintEvent * event) override;

    Ui::BulletinItem * ui;

    int day;
    int calories;
    std::vector<QColor> goals;
};

#endif // BULLETINITEM_H
