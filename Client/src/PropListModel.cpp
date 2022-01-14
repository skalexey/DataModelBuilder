#include <limits>
#include <QQmlApplicationEngine>
#include "PropListModel.h"
#include "TypeModel.h"
#include "TypeListModel.h"
#include "Utils.h"
#include "vl.h"


QVariantList TypePropListModel::typeList = composeTypeList();

TypePropListModel::Type TypePropListModel::GetVLType(const vl::Var &var)
{
    if (var.IsBool())
        return Type::Bool;
    else if (var.IsNumber())
        return Type::Int;
    else if (var.IsString())
        return Type::String;
    else if (var.IsList())
        return Type::List;
    else if (var.IsObject())
        return Type::Object;
    return Type::Null;
}

vl::VarPtr TypePropListModel::MakeVar(Type type)
{
    switch (type) {
        case Type::String:
            return std::make_shared<vl::StringVar>();
        case Type::Int:
            return std::make_shared<vl::NumberVar>();
        case Type::Bool:
            return std::make_shared<vl::BoolVar>();
        case Type::Object:
            return std::make_shared<vl::ObjectVar>();
        case Type::List:
            return std::make_shared<vl::ListVar>();
        case Type::Null:
        default:
            return std::make_shared<vl::NullVar>();
    }
}

QVariantList TypePropListModel::composeTypeList()
{
    QVariantList list;
    auto& metaObj = TypePropListModel::staticMetaObject;
    auto enumIndex = metaObj.indexOfEnumerator("Type");
    auto en = metaObj.enumerator(enumIndex);
    auto count = en.keyCount();
    for (int i = 0; i < count; i++)
        list.append(QString(en.key(i)));
    return list;
}

TypePropListModel::TypePropListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    resetList();
}

TypePropListModel::TypePropListModel(TypeModel *objectPtr)
{
    Init(objectPtr);
}

void TypePropListModel::Init(TypeModel* objectPtr)
{
    mObject = objectPtr;
    resetList([&]() {
        mList.clear();
        if (mObject)
            mObject->getData().ForeachProp([&](const std::string& propName, vl::Var& prop) {
                mList.push_back(propName);
                return true;
            });
    });
}

// ====== Begin of QAbstractListModel interface ======
int TypePropListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return mList.size();
}

QVariant TypePropListModel::data(const QModelIndex &index, int role) const
{
    int intIndex = index.row();
    if (!index.isValid() || mList.size() <= intIndex)
        return QVariant();
    auto& id = mList[intIndex];
    switch (role) {
        case RoleName:
            return QVariant(id.c_str());
        case RoleType:
            if (mObject)
                return QVariant(TypePropListModel::GetVLType(mObject->getData().Get(id)));
            return QVariant(TypePropListModel::Type::Null);
    }
    return QVariant();
}

bool TypePropListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int intIndex = index.row();
    if (intIndex >= mList.size())
        return false;
    auto& id = mList[intIndex];
    if (data(index, role) != value) {
        switch (role) {
            case RoleName:
                if (mObject)
                {
                    if (mObject->getData().RenameProperty(id, value.toString().toStdString()))
                        id = value.toString().toStdString();
                    else
                        emit nameAlreadyTaken();
                }
                break;
            case RoleType:
                if (mObject)
                {
                    auto& data = mObject->getData();
                    if (data.Has(id))
                    {
                        auto type = qvariant_cast<TypePropListModel::Type>(value);
                        if (TypePropListModel::GetVLType(data.Get(id)) != type)
                        {
                            data.Set(id, TypePropListModel::MakeVar(type));
                            return true;
                        }
                    }
                }
                break;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TypePropListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> TypePropListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoleName] = "name";
    roles[RoleType] = "type";
    return roles;
}
// ====== End of QAbstractListModel interface ======

void TypePropListModel::resetList(const std::function<void()>& doWithList)
{
    beginResetModel();
    if (doWithList)
        doWithList();
    else
        mList.clear();
    endResetModel();
}

bool TypePropListModel::addParam(const std::string &name, TypePropListModel::Type type)
{
    if (mObject)
    {
        auto& data = mObject->getData();
        std::string paramName = name;
        bool success = true;
        if (data.Has(paramName))
        {
            std::string paramNameTpl = Utils::FormatStr("%s %d", name.c_str());
            for (int i = 1; i < std::numeric_limits<int>::max(); i++)
                if ((success = !data.Has(paramName = Utils::FormatStr("New param %d", i))))
                    break;
        }
        if (success)
        {
            data.Set(paramName, TypePropListModel::MakeVar(type));
            auto sz = mList.size();
            beginInsertRows(QModelIndex(), sz, sz);
            mList.push_back(paramName);
            endInsertRows();
            return true;
        }
    }
    return false;
}

void TypePropListModel::removeParam(int index)
{
    if (index >= 0 && index < mList.size())
    {
        beginRemoveRows(QModelIndex(), index, index);
        auto& id = mList[index];
        if (mObject)
            mObject->getData().RemoveProperty(id);
        mList.erase(mList.begin() + index);
        endRemoveRows();
    }
}

//ObjectParameter *ObjectParameterListModel::getParam(int index)
//{
//    if (index >= 0 && index < mList.size())
//        return mList[index].get();
//    return nullptr;
//}
