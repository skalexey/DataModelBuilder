#include <string>
#include "VLVarModel.h"
#include "vl.h"
#include "VLObjectVarModel.h"
#include "VLListVarModel.h"
#include "VarModelFactory.h"
#include "DMBModel.h"

namespace dmb
{
	VLVarModelPtr nullVarModelPtr = nullptr;
	std::string emptyString;

	VLVarModel::VLVarModel(QObject *parent)
		: QObject(parent)
	{
//		qDebug() << "Create VarModel " << this;
	}

	VLVarModel::VLVarModel(const vl::Var& v, QObject *parent)
		: VLVarModel(parent)
	{
		mVarPtr = vl::MakePtr(v);
	}

	VLVarModel::~VLVarModel()
	{
//		qDebug() << "Destroy VarModel " << this;
		setParent(nullptr);
	}

	void VLVarModel::Init(QObject *parent)
	{
		setParent(parent);
	}

	void VLVarModel::Init(QObject *parent, const vl::Var &data, DMBModel* owner)
	{
		setParent(parent);
		mVarPtr = vl::MakePtr(data);
		setDataModel(owner);
	}

	void VLVarModel::Init(const VLVarModelPtr& parent)
	{
		setParent(parent.get());
		if (mVarPtr)
			mVarPtr.reset();
		mParentModel = parent;
		if (parent)
			setDataModel(parent->mDMBModel);
	}

	const VLObjectVarModel* VLVarModel::asObject() const
	{
		return nullptr;
	}

	VLObjectVarModel* VLVarModel::asObject()
	{
		return nullptr;
	}

	const VLListVarModel *VLVarModel::asList() const
	{
		return nullptr;
	}

	VLListVarModel *VLVarModel::asList()
	{
		return nullptr;
	}

	bool VLVarModel::isObject() const
	{
		return false;
	}

	bool VLVarModel::isList() const
	{
		return false;
	}

	const vl::Var &VLVarModel::getData() const
	{
		if (mVarPtr)
			return *mVarPtr.get();
		else if (auto parent = getParentModel())
			return parent->getChildData(this);
		return vl::emptyVar;
	}

	vl::Var &VLVarModel::getData()
	{
		return const_cast<vl::Var&>(const_cast<const VLVarModel*>(this)->getData());
	}

	const vl::Var &VLVarModel::getChildData(const VLVarModel *childPtr) const
	{
		// Only objects and lists can use this method
		return vl::emptyVar;
	}

	std::string VLVarModel::getTypeId() const
	{
		if (mDMBModel)
			return mDMBModel->getDataModel().GetTypeId(getData().AsObject());
		return "";
	}

	const DMBModel *VLVarModel::getDataModel() const
	{
		return mDMBModel;
	}

	DMBModel *VLVarModel::getDataModel()
	{
		return mDMBModel;
	}

	void VLVarModel::setDataModel(DMBModel *m)
	{
		mDMBModel = m;
		emit ownerChanged(m);
	}

	vl::Var &VLVarModel::getChildData(const VLVarModel *childPtr)
	{
		return const_cast<vl::Var&>(const_cast<const VLVarModel*>(this)->getChildData(childPtr));
	}

	VLVarModelPtr VLVarModel::getParentModel() const
	{
		return mParentModel.lock();
	}

	VLVarModelPtr VLVarModel::parentModel()
	{
		return mParentModel.lock();
	}

	QVariant VLVarModel::parentModelProp()
	{
		return QVariant::fromValue(mParentModel.lock().get());
	}

	bool VLVarModel::removeFromParent()
	{
		if (auto parent = getParentModel())
			if (auto owner = getDataModel())
			{
				auto ptr = parent->getChildPtr(this);
				owner->storeStandaloneModel(ptr);
				parent->removeChild(this);
				return true;
			}
		return false;
	}

	QVariant VLVarModel::copy()
	{
		if (auto owner = getDataModel())
			return QVariant::fromValue(
						owner->storeStandaloneModel(
							VarModelFactory::Instance().Create(*getData().Copy())
							).get()
						);
		return QVariant();
	}

	bool VLVarModel::setParentModel(const QVariant& data)
	{
		VLVarModel* newParent = nullptr;
		if (data.canConvert<VLVarModel*>())
			newParent = qvariant_cast<VLVarModel*>(data);

		auto currentParent = getParentModel();
		auto currentId = getId();
		if (removeFromParent())
			if (auto ptr = getDataModel()->takeStandaloneModel(this))
			{
				if (auto newParentObject = newParent->asObject())
					newParentObject->setModel(newParentObject->getFreeId(currentId), ptr);
				else if (newParent->isList())
					newParent->asList()->addModel(ptr);
				else
					return false;
				emit parentChanged();
				return true;
			}
		return false;
	}

	int VLVarModel::getIndex() const
	{
		if (auto parent = getParentModel())
			return parent->getChildIndex(this);
		return -1;
	}

	int VLVarModel::getChildIndex(const VLVarModel *childPtr) const
	{
		// Only objects and lists can use this method
		return -1;
	}

	bool VLVarModel::removeChild(const VLVarModel *childPtr)
	{
		// Only objects and lists can use this method
		return false;
	}

	const VLVarModelPtr &VLVarModel::getChildPtr(const VLVarModel *p) const
	{
		// Only objects and lists can use this method
		return nullVarModelPtr;
	}

	VLVarModelPtr VLVarModel::getPtr()
	{
		if (auto parent = getParentModel())
			return parent->getChildPtr(this);
		else // Objects and lists can return shared_from_this
			return nullptr;
	}

	bool VLVarModel::remove()
	{
		emit beforeRemove();
		if (auto parent = parentModel())
			return parent->removeChild(this);
		else if (auto owner = getDataModel())
			return owner->removeStandaloneModel(this);
		return false;
	}

	QString VLVarModel::id() const
	{
		return QString(getId().c_str());
	}

	bool VLVarModel::setId(const QString &newId)
	{
		return setId(newId.toStdString());
	}

	bool VLVarModel::setId(const std::string &newId)
	{
		// TODO: make warnings about proto changing
		if (auto parent = parentModel())
			if (parent->isObject())
				if (parent->asObject()->renameProperty(getId(), newId))
					return true; // idChanged signal emitted by renameProperty call above
		return false;
	}

	ObjectProperty::Type VLVarModel::type() const
	{
		return ObjectProperty::ConvertVLType(getData());
	}

	QString VLVarModel::typeStr() const
	{
		return ObjectProperty::GetTypeStr(getData());
	}

	void VLVarModel::setType(const ObjectProperty::Type &newType)
	{
		if (auto parent = parentModel())
		{
			if (parent->isObject())
				parent->asObject()->propListModel()->setType(getIndex(), newType);
			else if (parent->isList())
				parent->asList()->listModel()->setType(getIndex(), newType);
		}
	}

	QVariant VLVarModel::value() const
	{
		auto& v = getData();
		if (v.IsObject())
			return QVariant::fromValue(const_cast<VLVarModel*>(this));
		else if (v.IsBool())
			return QVariant(v.AsBool().Val());
		else if (v.IsString())
			return QVariant(v.AsString().Val().c_str());
		else if (v.IsNumber())
			return QVariant(v.AsNumber().Val());
		else if (v.IsList())
			return QVariant::fromValue(const_cast<VLVarModel*>(this));
		else
			return QVariant(v.ToStr().c_str());
	}

	bool VLVarModel::setValue(const QVariant &newValue)
	{
		return setValueInternal(newValue, true);
	}

	bool VLVarModel::setValueInternal(const QVariant &newValue, bool emitValueChanged)
	{
		auto& v = getData();
		if (v.IsString())
			v.AsString() = newValue.toString().toStdString();
		else if (v.IsNumber())
			v.AsNumber() = newValue.toInt();
		else if (v.IsBool())
			v.AsBool() = newValue.toBool();
		if (emitValueChanged)
			emit valueChanged(getIndex());
		return true;
	}

	const std::string &VLVarModel::getId() const
	{
		if (auto parent = getParentModel())
			if (parent->isObject())
				return parent->asObject()->getChildId(this);
		return emptyString;
	}

	QVariant VLVarModel::valueStr() const
	{
		return QVariant(getData().ToStr().c_str());
	}
}
