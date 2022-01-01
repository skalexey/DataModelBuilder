#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include <functional>
#include <memory>
#include "object.h"
#include "parameterlistmodel.h"

typedef Object ObjectData;
typedef std::shared_ptr<ObjectData> ObjectDataPtr;
typedef QVector<ObjectDataPtr> ObjectList;

class ObjectListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ObjectListModel(QObject *parent = nullptr);
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
    void resetList(const std::function<void()>& doWithList);

    Q_INVOKABLE ObjectData* object(int index) const;
    Q_INVOKABLE void newObject();
    Q_INVOKABLE void removeObject(int index);

private:
    enum {
        NameRole,
        ParamsRole
    };

private:
    ObjectList mList;
};

#endif // OBJECTLISTMODEL_H
