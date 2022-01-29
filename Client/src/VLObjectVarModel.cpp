#include <QObject>
#include "VLObjectVarModel.h"
#include "VLCollectionModel.h"
#include "vl.h"
#include "VLListModel.h"
#include "VLVarModel.h"

namespace dmb
{
	VLObjectVarModel* emptyObjectModel = new VLObjectVarModel;
	const std::string emptyString;

	VLObjectVarModel::VLObjectVarModel(QObject *parent)
		: Base(parent)
	{
//		if (parent)
//			qDebug() << "Object(QObject* parent) Object with a parent created";
//		else
//			qDebug() << "Object(QObject* parent == nullptr) Empty object cretaed";
	}

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

	void VLObjectVarModel::Init(QObject *parent, const vl::Var& data)
	{
		Base::Init(parent, data);
		mPropListModel.Init(std::dynamic_pointer_cast<VLVarModel>(shared_from_this()));
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

	// Data and model getters
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

	const VLVarModel *VLObjectVarModel::getModel(const std::string &propId) const
	{
		return mPropListModel.getModel(propId);
	}

	VLVarModel *VLObjectVarModel::getModel(const std::string &propId)
	{
		return mPropListModel.getModel(propId);
	}

	VLCollectionModel* VLObjectVarModel::propListModel()
	{
		if (!mPropListModel.elementsLoaded())
			loadPropList();
		return &mPropListModel;
	}

	VLCollectionModel *VLObjectVarModel::protoPropListModel()
	{
		if (auto protoModel = getModel("proto"))
			if (auto objectModel = protoModel->asObject())
				return objectModel->propListModel();
		return nullptr;
	}

	void VLObjectVarModel::add(const QString& propId, ObjectProperty::Type type)
	{
		mPropListModel.add(propId, type);
	}

	bool VLObjectVarModel::remove(const QString& propName)
	{
		return mPropListModel.remove(propName);
	}

	bool VLObjectVarModel::removeAt(int index)
	{
		return mPropListModel.removeAt(index);
	}

	bool VLObjectVarModel::has(const QString &propId) const
	{
		return getData().Has(propId.toStdString());
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

	vl::Var& VLObjectVarModel::setData(const std::string &propId, const vl::Var &v)
	{
		return mPropListModel.setData(propId, v);
	}
}
