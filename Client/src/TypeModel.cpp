#include "TypeModel.h"
#include "PropListModel.h"
#include "TypeListModel.h"
#include "vl.h"

namespace
{
    vl::Object emptyObject;
    std::string emptyString;
}

TypeModel::TypeModel(TypeListModel* listModelPtr, int index)
    : mParamListModel(this)
{
    Init(listModelPtr, index);
    qDebug() << "Object(listModelPtr, " << mIndex << ")";
}

TypeModel::TypeModel(QObject *parent) : QObject(parent)
{
    if (parent)
        qDebug() << "Object(parent) Object with a parent created";
    else
        qDebug() << "Object() Empty object cretaed";
}

TypeModel::~TypeModel() {
    qDebug() << "~Object() with index " << mIndex << "";
}

void TypeModel::Init(TypeListModel *modelPtr, int index)
{
    mListModel = modelPtr;
    mIndex = index;
    mParamListModel.Init(this);
}

vl::Object &TypeModel::getData() const
{
    if (!mListModel)
        return emptyObject;
    return mListModel->getDataModel().GetRegistry().GetType(getId());
}

const std::string &TypeModel::getId() const
{
    if (!mListModel)
        return emptyString;
    return mListModel->getObjectId(mIndex);
}

QString TypeModel::name() const
{
    return QString(getId().c_str());
}

void TypeModel::setName(const QString &newName)
{
    if (mListModel == nullptr)
        return;
    auto& id = getId();
    std::string newId = newName.toStdString();
    if (!mListModel->getDataModel().GetRegistry().RenameType(id, newId))
        return;
    emit nameChanged();
}

TypePropListModel* TypeModel::paramListModel()
{
    return &mParamListModel;
}

void TypeModel::newParam()
{
    mParamListModel.addParam("New param", TypePropListModel::String);
}

void TypeModel::removeParam(int index)
{
    mParamListModel.removeParam(index);
}
