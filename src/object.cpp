#include "object.h"
#include "parameterlistmodel.h"

Object::Object(const QString& name)
    : mName(name)
    , mParamListModel(this)
{}

ObjectParameterListModel* Object::paramListModel()
{
    return &mParamListModel;
}

void Object::newParam()
{
    mParamListModel.addParam("New param", ObjectParameter::TypeString);
}

void Object::removeParam(int index)
{
    mParamListModel.removeParam(index);
}

ObjectParameter *Object::param(int index)
{
    return mParamListModel.getParam(index);
}
