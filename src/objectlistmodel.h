#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include <functional>
#include <memory>
#include "object.h"
#include "parameterlistmodel.h"

typedef Object<QString> ObjectData;
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

public slots:
    void addItem();
    void removeItem(int index);

private:
    enum {
        NameRole,
        ParamsRole
    };

private:
    ObjectList mList;
};

#endif // OBJECTLISTMODEL_H
