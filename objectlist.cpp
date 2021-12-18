#include "objectlist.h"
Q_DECLARE_METATYPE(ObjectParam);

ObjectList::ObjectList(QObject *parent)
    : QObject{parent}
{
    mItems.append({"Item One", {{"Name", ParamTypeString}, {"Used", ParamTypeFlag}} });
    mItems.append({"Item Two", {{"Title", ParamTypeString}, {"Image", ParamTypeImage}} });
    mItems.append({"Item Three", {{"Param 1", ParamTypeString}, {"Description", ParamTypeString}, {"Image", ParamTypeImage}} });
}

bool ObjectList::setItemAt(int index, const ObjectListItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;
    auto& currentItem = mItems[index];
    if (currentItem == item)
        return false;
    mItems[index] = item;
    return true;
}

QVector<ObjectListItem> ObjectList::items() const
{
    return mItems;
}

void ObjectList::appendItem()
{
    emit preItemAppended();
    mItems.append({"New Item", {{"Param 1", ParamTypeString}, {"Description", ParamTypeString}, {"Image", ParamTypeImage}} });
    emit postItemAppended();
}

bool ObjectListItem::operator ==(const ObjectListItem &right) const
{
    return name == right.name
            && params == right.params;
}

QVariantList ObjectListItem::paramsQVariantList() const
{
    QVariantList result;
    for (auto& param : params) {
        QVariant var;
        var.setValue(param);
        result.append(var);
    }
    return result;
}

bool ObjectParam::operator ==(const ObjectParam &right) const
{
    return name == right.name
        && type == right.type;
}
