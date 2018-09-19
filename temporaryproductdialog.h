#ifndef TEMPORARYPRODUCTDIALOG_H
#define TEMPORARYPRODUCTDIALOG_H

#include "abstractproduct.h"

#include <QDialog>

namespace Ui {
class TemporaryProductDialog;
}

class TemporaryProductDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TemporaryProductDialog(QWidget *parent = nullptr);
    ~TemporaryProductDialog();

    std::pair<std::string, ProductParams> get_result() const;

private slots:
    void ok_pressed();

private:
    Ui::TemporaryProductDialog *ui;
    ProductParams params;
    std::string name;
};

#endif // TEMPORARYPRODUCTDIALOG_H
