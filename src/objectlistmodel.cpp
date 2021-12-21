#include "objectlistmodel.h"
#include "parameterlistmodel.h"

namespace
{
    ObjectParameterListModel emptyParamListModel;
    ObjectData emptyObject;
}

ObjectListModel::ObjectListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    resetList([&] {
        mList.append(std::make_shared<ObjectData>("Scheme element"));
        mList.back()->paramListModel()->addParam("Title", ObjectParameter::TypeString);
        mList.back()->paramListModel()->addParam("Image", ObjectParameter::TypeImage);
        mList.append(std::make_shared<ObjectData>("Connection"));
        mList.back()->paramListModel()->addParam("Title", ObjectParameter::TypeString);
        mList.back()->paramListModel()->addParam("Doublesided", ObjectParameter::TypeFlag);
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
            return QVariant(item.name());
        case ParamsRole:
            return QVariant::fromValue(item.paramListModel());
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
            item.setName(value.toString());
            break;
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
    mList.back()->paramListModel()->addParam("Param 1", ObjectParameter::TypeString);
    mList.back()->paramListModel()->addParam("Description", ObjectParameter::TypeString);
    mList.back()->paramListModel()->addParam("Image", ObjectParameter::TypeImage);
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

ObjectData *ObjectListModel::object(int index) const
{
    auto sz = mList.size();
    if (index < sz && index >= 0)
        return mList[index].get();
    else
        return &emptyObject;
}

