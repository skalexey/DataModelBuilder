#include "parameterlistmodel.h"
#include "objectparameter.h"

ObjectParameterListModel::ObjectParameterListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    resetList([&] {
        mList.append({"Param 1", ParamTypeString});
        mList.append({"Param 2", ParamTypeFlag});
        mList.append({"Param 3", ParamTypeImage});
    });
}

// ====== Begin of QAbstractListModel interface ======
int ObjectParameterListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return mList.size();
}

QVariant ObjectParameterListModel::data(const QModelIndex &index, int role) const
{
    int intIndex = index.row();
    if (!index.isValid() || mList.size() <= intIndex)
        return QVariant();
    auto& item = mList[intIndex];
    switch (role) {
        case RoleName:
            return QVariant(item.name);
        case RoleType:
            return QVariant(item.type);
    }
    return QVariant();
}

bool ObjectParameterListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int intIndex = index.row();
    if (intIndex >= mList.size())
        return false;
    auto& item = mList[intIndex];
    if (data(index, role) != value) {
        switch (role) {
            case RoleName:
                item.name = value.toString();
                break;
            case RoleType:
                item.type = qvariant_cast<ObjectParamType>(value);
                break;
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ObjectParameterListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> ObjectParameterListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoleName] = "name";
    roles[RoleType] = "type";
    return roles;
}
// ====== End of QAbstractListModel interface ======

void ObjectParameterListModel::resetList(const std::function<void()>& doWithList)
{
    beginResetModel();
    if (doWithList)
        doWithList();
    else
        mList.clear();
    endResetModel();
}
