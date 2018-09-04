#ifndef SORTEDNAMES_H
#define SORTEDNAMES_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>

class SortedNames : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SortedNames(QObject * parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // My API
    bool insert_row(const QString & row);
    bool remove_row(const QString & row);

private:
    std::vector<QString> names;
};

#endif // SORTEDNAMES_H
