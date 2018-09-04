#include "ingredientcompleterdelegate.h"
#include "productdictionary.h"

#include <QCompleter>
#include <QLineEdit>

IngredientCompleterDelegate::IngredientCompleterDelegate(ProductDictionary & product_dict, QObject *parent)
    : QStyledItemDelegate{ parent }, dict_names{ product_dict.get_names() }
{}

QWidget * IngredientCompleterDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    auto editor = new QLineEdit(parent);

    if(index.column() == 0)
    {
        QCompleter * q = new QCompleter(dict_names);
        q->setCaseSensitivity(Qt::CaseInsensitive);
        q->setCompletionMode(QCompleter::PopupCompletion);
        q->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        editor->setCompleter(q);
    }

    return editor;
}

void IngredientCompleterDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    auto value = index.model()->data(index, Qt::EditRole).toString();
    auto le = static_cast<QLineEdit * >(editor);
    le->setText(value);
}

void IngredientCompleterDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    auto le = static_cast<QLineEdit * >(editor);
    model->setData(index, le->text(), Qt::EditRole);
}

void IngredientCompleterDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
