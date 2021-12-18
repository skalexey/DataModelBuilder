#include "objectmodel.h"
#include "objectlist.h"

ObjectListModel::ObjectListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    setList(new ObjectList());
}

ObjectListModel::~ObjectListModel()
{
    delete mList;
}

int ObjectListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    auto item = mList->items().at(index.row());
    switch (role)
    {
        case NameRole:
            return QVariant(item.name);
        case ParamsRole:
            return QVariant(item.paramsQVariantList());
    }
    return QVariant();
}

bool ObjectListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;
    auto item = mList->items().at(index.row());
    switch (role)
    {
        case NameRole:
            item.name = value.toString();
            break;
        case ParamsRole: {
            item.params.clear();
            auto qvarlist = value.toList();
            for (auto& v : qvarlist)
                item.params.append(qvariant_cast<ObjectParam>(v));
        }
    }
    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

void ObjectListModel::addItem()
{
    if (mList)
        mList->appendItem();
}

Qt::ItemFlags ObjectListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ObjectListModel::roleNames() const
{
    QHash<int, QByteArray> result;
    result[NameRole] = "name";
    result[ParamsRole] = "params";
    return result;
}

void ObjectListModel::setList(ObjectList *list)
{
    beginResetModel();
    if (mList)
        mList->disconnect(this);
    mList = list;
    if (mList) {
        connect(mList, &ObjectList::preItemAppended, this, [=] () {
            auto index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &ObjectList::postItemAppended, this, [=] () {
            endInsertRows();
        });
        connect(mList, &ObjectList::preItemRemoved, this, [=] (int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &ObjectList::postItemRemoved, this, [=] () {
            endRemoveRows();
        });
    }
    endResetModel();
}
