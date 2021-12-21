#ifndef PARAMETERLISTMODEL_H
#define PARAMETERLISTMODEL_H

#include <QAbstractListModel>
#include <QVariantList>
#include <QVector>
#include <functional>
#include "objectparameter.h"

typedef ObjectParameter<QString> ObjectParameterData;
typedef QVector<ObjectParameterData> ParameterList;

class ObjectParameterListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUM (ObjectParamType)
    enum Role {
        RoleName,
        RoleType
    };

public:
    explicit ObjectParameterListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int,QByteArray> roleNames() const override;
    // Custom:
    void resetList(const std::function<void()>& doWithList = nullptr);
    void addParam(const QString& name, ObjectParamType type);
private:
    ParameterList mList;
};

#endif // PARAMETERLISTMODEL_H
