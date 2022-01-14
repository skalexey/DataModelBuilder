#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include "TypeModel.h"
#include "PropListModel.h"
#include "DMBCore.h"

typedef TypeModel ObjectData;
typedef std::shared_ptr<ObjectData> ObjectDataPtr;

// object id is only c-string
typedef std::vector<std::string> ObjectIdList;

class TypeListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TypeListModel(QObject *parent = nullptr);
    ~TypeListModel();

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
    dmb::Model& getDataModel();
    void resetList(const std::function<void()>& doWithList);
    const std::string& getObjectId(int index);

    // Properties
    Q_INVOKABLE ObjectData* object(int index);
    Q_INVOKABLE void newObject();
    Q_INVOKABLE void removeObject(int index);
    Q_INVOKABLE void store();

private:
    enum {
        NameRole,
    };

private:
    ObjectIdList mList;
    dmb::Model mModel;
    ObjectData mObject;
};

#endif // OBJECTLISTMODEL_H
