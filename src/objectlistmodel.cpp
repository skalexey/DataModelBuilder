#include "objectlistmodel.h"
#include "parameterlistmodel.h"

ObjectListModel::ObjectListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    resetList([&] {
        mList.append(std::make_shared<ObjectData>("Item One"));
        mList.append(std::make_shared<ObjectData>("Item Two"));
        mList.append(std::make_shared<ObjectData>("Item Three"));
        mList.append(std::make_shared<ObjectData>("Item Four"));
//        mList.append({"Item One", {{"Name", ObjectParameterModel::ParamTypeString}, {"Used", ObjectParameterModel::ParamTypeFlag}} });
//        mList.append({"Item Two", {{"Title", ObjectParameterModel::ParamTypeString}, {"Image", ObjectParameterModel::ParamTypeImage}} });
//        mList.append({"Item Three", {{"Param 1", ObjectParameterModel::ParamTypeString}, {"Description", ObjectParameterModel::ParamTypeString}, {"Image", ObjectParameterModel::ParamTypeImage}} });
    });
}

int ObjectListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return mList.size();
}

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
    auto rowIndex = index.row();
    if (!index.isValid() || mList.size() <= rowIndex)
        return QVariant();

    auto& item = *mList[rowIndex];
    switch (role)
    {
        case NameRole:
            return QVariant(item.name);
        case ParamsRole:
            return QVariant::fromValue(&item.paramListModel());
    }
    return QVariant();
}

bool ObjectListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int rowIndex = index.row();
    if (mList.empty() || mList.size() <= rowIndex)
        return false;
    auto& item = *mList[rowIndex];
    switch (role)
    {
        case NameRole:
            item.name = value.toString();
            break;
        //case ParamsRole: {
            //return qvariant_cast<ObjectParameterListModel>(v));
           default:
            return false;
    }
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

void ObjectListModel::addItem()
{
    auto sz = mList.size();
    beginInsertRows(QModelIndex(), sz, sz);
    mList.append(std::make_shared<ObjectData>("new item"));
//    mList.append({"Very New Item",
//                  {{"Param 1", ObjectParameterModel::ParamTypeString}
//                   , {"Description", ObjectParameterModel::ParamTypeString}
//                   , {"Image", ObjectParameterModel::ParamTypeImage}
//    }});
    endInsertRows();
}

void ObjectListModel::removeItem(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    mList.remove(index);
    endRemoveRows();
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

void ObjectListModel::resetList(const std::function<void()>& doWithList)
{
    beginResetModel();
    if (doWithList != nullptr)
        doWithList();
    else
        mList.clear();
    endResetModel();
}

//bool ObjectList::setItemAt(int index, const ObjectListItem &item)
//{
//    if (index < 0 || index >= mItems.size())
//        return false;
//    auto& currentItem = mItems[index];
//    if (currentItem == item)
//        return false;
//    mItems[index] = item;
//    return true;
//}
