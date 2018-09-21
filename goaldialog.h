#ifndef GOALDIALOG_H
#define GOALDIALOG_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class GoalDialog;
}

class GoalDialog : public QDialog
{
    Q_OBJECT

public:
    using item_type = std::pair<std::string, QColor>;

    explicit GoalDialog(const item_type & old_item = std::make_pair("", Qt::white),
                        QWidget *parent = nullptr);
    ~GoalDialog();

    item_type get_result() const;

private slots:
    void ok_pushed();
    void color_chosen();

private:
    void set_background(const QColor & color);

    item_type data;
    Ui::GoalDialog *ui;
    QColorDialog color_dialog;
};

#endif // GOALDIALOG_H
