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
    explicit GoalDialog(QWidget *parent = nullptr);
    ~GoalDialog();

    std::pair<std::string, QColor> get_result() const;

private slots:
    void ok_pushed();
    void color_chosen();

private:
    std::pair<std::string, QColor> data;
    Ui::GoalDialog *ui;
    QColorDialog color_dialog;
};

#endif // GOALDIALOG_H
