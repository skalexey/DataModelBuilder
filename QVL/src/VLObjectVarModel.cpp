#include <QObject>
#include "VLObjectVarModel.h"
#include "VLCollectionModel.h"
#include "VLListModel.h"
#include "VLVarModel.h"
#include "vl.h"
#include "DMBModel.h"
#include "Utils.h"

namespace dmb
{
	const std::string emptyString;

	VLObjectVarModel::VLObjectVarModel(QObject *parent)
		: Base(parent)
	{
//		if (parent)
//			qDebug() << "Object(QObject* parent) Object with a parent created";
//		else
//			qDebug() << "Object(QObject* parent == nullptr) Empty object cretaed";
	}

	VLObjectVarModel::VLObjectVarModel(const vl::Var& v, QObject* parent)
		: Base(v, parent)
	{}

	VLObjectVarModel::~VLObjectVarModel() {
		//qDebug() << "~Object() with id '" << Base::getId().c_str() << "'";
	}

	bool VLObjectVarModel::loadPropList()
	{
//		qDebug() << "Load element models of object '" << Base::getId().c_str() << "' ";
		bool result = mPropListModel.loadElementModels();
//		if (result)
//			qDebug() << " loaded\n";
//		else
//			qDebug() << " not loaded\n";
		return result;
	}

	void VLObjectVarModel::Init(QObject *parent)
	{
		Base::Init(parent);
		mPropListModel.setParent(this);
	}

	void VLObjectVarModel::Init(QObject *parent, const vl::Var& data, DMBModel* owner)
	{
		Base::Init(parent, data, owner);
		mPropListModel.Init(shared_from_this());
		// Uncomment if there are problems with not existent nested models
		// Only in the first call of the nested model
		//loadPropList();
	}

	void VLObjectVarModel::Init(const VLVarModelPtr& parent)
	{
		Base::Init(parent);
		mPropListModel.Init(shared_from_this());
	}

	// Type casts
	const VLObjectVarModel *VLObjectVarModel::asObject() const
	{
		return this;
	}

	VLObjectVarModel *VLObjectVarModel::asObject()
	{
		return this;
	}

	// Type check
	bool VLObjectVarModel::isObject() const
	{
		return true;
	}

	bool VLObjectVarModel::has(const QString &propId) const
	{
		return has(propId.toStdString());
	}

	bool VLObjectVarModel::has(const std::string &propId) const
	{
		return getData().Has(propId);
	}

	const vl::ObjectVar &VLObjectVarModel::getData() const
	{
		return Base::getData().AsObject();
	}

	vl::ObjectVar &VLObjectVarModel::getData()
	{
		return const_cast<vl::ObjectVar&>(const_cast<const VLObjectVarModel*>(this)->getData());
	}

	const std::string &VLObjectVarModel::getPropId(int index) const
	{
		return mPropListModel.getId(index);
	}

	const VLVarModel *VLObjectVarModel::getAt(int index) const
	{
		return mPropListModel.getAt(index);
	}

	VLVarModel *VLObjectVarModel::getAt(int index)
	{
		return const_cast<VLVarModel*>(const_cast<const VLObjectVarModel*>(this)->getAt(index));
	}

	int VLObjectVarModel::getChildIndex(const VLVarModel *childPtr) const
	{
		return mPropListModel.getElementIndex(childPtr);
	}

	const std::string &VLObjectVarModel::getChildId(const VLVarModel *childPtr) const
	{
		auto index = getChildIndex(childPtr);
		if (index >= 0)
			return getId(index);
		return emptyString;
	}

	bool VLObjectVarModel::removeChild(const VLVarModel *childPtr)
	{
		return mPropListModel.remove(getChildId(childPtr));
	}

	const std::string &VLObjectVarModel::getId(int index) const
	{
		return mPropListModel.getId(index);
	}

	int VLObjectVarModel::getIndex(const std::string &propId) const
	{
		return mPropListModel.getIndex(propId);
	}

	bool VLObjectVarModel::renameProperty(const std::string &propId, const std::string &newId)
	{
		return mPropListModel.renameElement(propId, newId);
	}

	const vl::Var &VLObjectVarModel::getChildData(const VLVarModel *childPtr) const
	{
		return mPropListModel.getDataAt(getChildIndex(childPtr));
	}

	const VLVarModelPtr& VLObjectVarModel::getModelSp(const std::string &propId) const
	{
		return mPropListModel.getModelSp(propId);
	}

	const VLVarModelPtr& VLObjectVarModel::modelSp(const std::string &propId)
	{
		return mPropListModel.modelSp(propId);
	}

	const VLVarModelPtr &VLObjectVarModel::setModel(const std::string &propId, const VLVarModelPtr &modelPtr)
	{
		return mPropListModel.setModel(propId, modelPtr);
	}

	const VLVarModel *VLObjectVarModel::getModel(const std::string &propId) const
	{
		return mPropListModel.getModel(propId);
	}

	VLVarModel *VLObjectVarModel::model(const std::string &propId)
	{
		return mPropListModel.model(propId);
	}

	VLCollectionModel* VLObjectVarModel::propListModel()
	{
		if (!mPropListModel.elementsLoaded())
			loadPropList();
		return &mPropListModel;
	}

	VLCollectionModel *VLObjectVarModel::protoPropListModel()
	{
		if (auto protoModel = model("proto"))
			if (auto objectModel = protoModel->asObject())
				return objectModel->propListModel();
		return nullptr;
	}

	VLCollectionModel& VLObjectVarModel::getPropListModel()
	{
		return mPropListModel;
	}

	void VLObjectVarModel::add(const QString& propId, ObjectProperty::Type type)
	{
		mPropListModel.add(propId, type);
	}

	bool VLObjectVarModel::removeProp(const QString& propName)
	{
		return mPropListModel.remove(propName);
	}

	bool VLObjectVarModel::removeAt(int index)
	{
		return mPropListModel.removeAt(index);
	}

	VLVarModel *VLObjectVarModel::at(int index)
	{
		return mPropListModel.at(index);
	}

	VLVarModel *VLObjectVarModel::get(const QString &propId)
	{
		return mPropListModel.get(propId);
	}

	VLVarModel *VLObjectVarModel::set(const QString &propId, VLVarModel *var)
	{
		return mPropListModel.set(propId, var);
	}

	VLVarModel *VLObjectVarModel::set(const QString &propId, const QVariant &data)
	{
		return mPropListModel.set(propId, data);
	}

	vl::Var& VLObjectVarModel::setData(const std::string &propId, const vl::Var &v)
	{
		return mPropListModel.setData(propId, v);
	}

	void VLObjectVarModel::instantiate(const QString& typeId, const QString& instanceName)
	{
		if (auto owner = getDataModel())
		{
			if (instanceName.isEmpty())
			{
				auto instId = Utils::FormatStr("Concrete_%s", typeId.toStdString().c_str());
				bool success = true;
				if (has(instId))
				{
					std::string tpl = instId;
					for (int i = 1; i < std::numeric_limits<short>::max(); i++)
						if ((success = !has(instId = Utils::FormatStr("%s %d", tpl.c_str(), i))))
							break;
				}
				if (success)
					emit instantiateRequested(QString(instId.c_str()), typeId);
				else
					emit instantiateRefused("Name error");
			}
			else
			{
				auto instId = instanceName.toStdString();
				if (has(instId))
					emit instantiateRefused("Name error");
				else
				{
					auto o = owner->createObjectSp();
					if (o->setPrototype(typeId))
					{
						setModel(instId, o);
						emit instantiated(typeId, instanceName);
					}
					else
						emit instantiateRefused("Type error");
				}
			}
		}
	}

	bool VLObjectVarModel::setPrototype(const QString& protoId)
	{
		return setPrototype(protoId.toStdString());
	}

	bool VLObjectVarModel::setPrototype(VLObjectVarModel *model)
	{
		if (auto modelPtr = model->shared_from_this())
			return setModel("proto", modelPtr) != nullptr;
		return false;
	}

	bool VLObjectVarModel::setPrototype(const std::string &protoId)
	{
		if (auto owner = getDataModel())
			if (auto modelPtr = owner->modelByTypeId(protoId))
				return setModel("proto", modelPtr) != nullptr;
		return false;
	}

	QVariant VLObjectVarModel::protoId() const
	{
		if (auto protoModel = getModel("proto"))
			return protoModel->asObject()->typeId();
		return QVariant();
	}

	QVariant VLObjectVarModel::typeId() const
	{
		return QVariant(QString(getTypeId().c_str()));
	}


}
