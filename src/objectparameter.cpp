#include "objectparameter.h"
#include <QQmlApplicationEngine>

QVariantList ObjectParameter::typeList = composeTypeList();

QVariantList ObjectParameter::composeTypeList()
{
    QVariantList list;
    auto& metaObj = ObjectParameter::staticMetaObject;
    auto enumIndex = metaObj.indexOfEnumerator("Type");
    auto en = metaObj.enumerator(enumIndex);
    auto count = en.keyCount();
    for (int i = 0; i < count; i++)
        list.append(QString(en.key(i)));
    return list;
}

ObjectParameter::Type ObjectParameter::type() const
{
    return mType;
}

void ObjectParameter::setType(ObjectParameter::Type newType)
{
    if (mType == newType)
        return;
    mType = newType;
    emit typeChanged();
}
