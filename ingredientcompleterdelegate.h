#ifndef INGREDIENTCOMPLETERDELEGATE_H
#define INGREDIENTCOMPLETERDELEGATE_H

#include <QStyledItemDelegate>

class ProductDictionary;

class IngredientCompleterDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    IngredientCompleterDelegate(ProductDictionary & product_dict, QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QAbstractItemModel * dict_names;
};

#endif // INGREDIENTCOMPLETERDELEGATE_H
