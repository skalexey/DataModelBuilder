#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "objectlist.h"

//class ObjectList;

class ObjectListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ObjectListModel(QObject *parent = nullptr);
    ~ObjectListModel();
    // QAbstractListModel interface
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
    // Our interface
    void setList(ObjectList* list);

public slots:
    void addItem();

private:
    enum {
        NameRole,
        ParamsRole
    };

    ObjectList* mList = nullptr;
};

#endif // OBJECTMODEL_H
