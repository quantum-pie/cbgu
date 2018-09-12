#ifndef CHECKABLELISTMODEL_H
#define CHECKABLELISTMODEL_H

#include "nlohmann/json.hpp"

#include <QAbstractListModel>
#include <QModelIndex>
#include <QColor>

#include <tuple>

using namespace nlohmann;

class CheckableListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CheckableListModel(QObject * parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // My API
    template<typename S, typename Q>
    void add_row(S&& name, Q&& color)
    {
        beginInsertRows(QModelIndex{}, 0, 0);
        insertRow(0);
        list.emplace_back(std::forward<S>(name), false, std::forward<Q>(color));
        endInsertRows();
    }

    bool remove_row(int position);
    json get_json() const;
    json get_goals() const;
    void clear();

private:
    std::vector<std::tuple<std::string, bool, QColor>> list;
};
#endif // CHECKABLELISTMODEL_H
