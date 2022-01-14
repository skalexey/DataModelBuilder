#include <limits>
#include<QDebug>
#include "TypeListModel.h"
#include "PropListModel.h"
#include "Utils.h"

namespace
{
    TypePropListModel emptyParamListModel;
    std::string emptyString;
}

TypeListModel::TypeListModel(QObject *parent)
    : QAbstractListModel(parent)
    , mObject(this)
{
    resetList([&] {
        if (!mModel.Load("model.json"))
            mModel.Store("model.json", { true });
        mModel.GetRegistry().ForeachType([&](const std::string& name, vl::Object& obj) {
            mList.push_back(name.c_str());
            return true;
        });
    });
    mObject.Init(this, -1);
}

TypeListModel::~TypeListModel()
{
    qDebug() << "~ObjectListModel()";
}

int TypeListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return mList.size();
}

QVariant TypeListModel::data(const QModelIndex &index, int role) const
{
    auto rowIndex = index.row();
    if (!index.isValid() || mList.size() <= rowIndex)
        return QVariant();

    auto& name = mList[rowIndex];
    switch (role)
    {
        case NameRole:
            return QVariant(QString(name.c_str()));
    }
    return QVariant();
}

bool TypeListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int rowIndex = index.row();
    if (mList.empty() || mList.size() <= rowIndex)
        return false;
    auto& id = mList[rowIndex];
    switch (role)
    {
        case NameRole:
        {
            std::string newId = value.toString().toStdString();
            if (mModel.GetRegistry().RenameType(id, newId))
                id = newId;
            break;
        }
        default:
            return false;
    }
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

void TypeListModel::newObject()
{
    auto sz = mList.size();
    beginInsertRows(QModelIndex(), sz, sz);
    auto obj = vl::Object();
    obj.Set("Param 1", "Default value");
    obj.Set("Description", "Default value 2");
    obj.Set("Image", "Default value 3");
    std::string objectName = "New object";
    auto& registry = mModel.GetRegistry();
    if (registry.HasType(objectName))
        for (int i = 1; i < std::numeric_limits<int>::max(); i++)
            if (!registry.HasType(objectName = Utils::FormatStr("New object %d", i)))
                break;
    if (mModel.GetRegistry().RegisterType(objectName, obj))
        mList.push_back(objectName);
    endInsertRows();
}

void TypeListModel::removeObject(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    if (index >= 0 && index < mList.size())
    {
        mModel.GetRegistry().RemoveType(mList[index]);
        mList.erase(mList.begin() + index);
    }
    endRemoveRows();
}

void TypeListModel::store()
{
    mModel.Store("model.json", { true });
}

dmb::Model &TypeListModel::getDataModel()
{
    return mModel;
}

Qt::ItemFlags TypeListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> TypeListModel::roleNames() const
{
    QHash<int, QByteArray> result;
    result[NameRole] = "name";
    return result;
}

void TypeListModel::resetList(const std::function<void()>& doWithList)
{
    beginResetModel();
    if (doWithList != nullptr)
        doWithList();
    else
        mList.clear();
    endResetModel();
}

const std::string &TypeListModel::getObjectId(int index)
{
    if (index >= 0 && index < mList.size())
        return mList[index];
    return emptyString;
}

ObjectData* TypeListModel::object(int index)
{
    auto sz = mList.size();
    //qDebug() << "ObjectListModel::object(" << index << ") from list with size " << sz;
    if (index < sz && index >= 0)
    {
        mObject.Init(this, index);
        return &mObject;
    }
    else
        return nullptr;
}

