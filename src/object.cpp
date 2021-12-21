#include "object.h"
#include "parameterlistmodel.h"

Object::Object(const QString& name)
    : mName(name)
    , mParamListModel(nullptr)
{}

ObjectParameterListModel* Object::paramListModel()
{
    return &mParamListModel;
}
