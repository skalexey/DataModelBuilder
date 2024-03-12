#include <string>
#include "VLVarModel.h"
#include "vl.h"
#include "VLObjectVarModel.h"
#include "VLListVarModel.h"
#include "VarModelFactory.h"
#include "DMBModel.h"
#include <utils/string_utils.h>
#include <utils/log.h>
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("VLVarModel")
LOG_STREAM([]{return qDebug();})
SET_LOCAL_LOG_LEVEL(debug)

namespace dmb
{
	VLVarModelPtr nullVarModelPtr = nullptr;
	std::string emptyString;

	VLVarModel::VLVarModel(QObject *parent)
		: QObject(parent)
	{
		setParent(nullptr);
	}

	VLVarModel::VLVarModel(const vl::Var& v, QObject *parent)
		: VLVarModel(parent)
	{
		mVarPtr = vl::MakePtr(v);
	}

	VLVarModel::~VLVarModel()
	{
		LOCAL_DEBUG("Destroy VarModel " << this);
		emit removed();
		LOCAL_DEBUG(this << "->setParent(nullptr)");
		setParent(nullptr);
	}

	void VLVarModel::Init(QObject *parent)
	{
		setParent(parent);
	}

	void VLVarModel::Init(QObject *parent, DMBModel* owner)
	{
		if (getParentModel())
		{
			if (auto& data = getData())
				mVarPtr = vl::MakePtr(data);
			mParentModel.reset();
		}
		setParent(parent);
		setDataModel(owner);
	}

	void VLVarModel::Init(QObject *parent, const vl::Var &data, DMBModel* owner)
	{
		mVarPtr = vl::MakePtr(data);
		Init(parent, owner);
	}


	void VLVarModel::Init(const VLVarModelPtr& parent)
	{
		LOCAL_DEBUG(this << "->setParent(" << parent.get() << ")");
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

	// Gets the data through the parent
	const vl::Var &VLVarModel::getData() const
	{
		if (mVarPtr)
			return *mVarPtr.get();
		else if (auto parent = getParentModel())
			return parent->getChildData(this);
		return vl::emptyVar;
	}

	// Non-const mirror
	vl::Var &VLVarModel::data()
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
			return mDMBModel->getDataModel().GetTypeId(getData().as<vl::Object>());
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

	// Detaches model and data from the parent
	// and keep both as a standalone model.
	// The model itself remains valid
	bool VLVarModel::detachFromParent()
	{
		if (auto parent = getParentModel())
			if (auto owner = getDataModel())
			{
				if (auto ptr = parent->getChild(this))
				{
					owner->storeStandaloneModel(ptr);
					parent->removeChild(this);
					return true;
				}
				else
				{
					LOCAL_ERROR("detachFromParent: can't find this as a child of it's parent (this: " << this << " , parent: " << parent.get() << "'");
				}
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
		if (!newParent)
			return false;

		auto currentParent = getParentModel();
		auto currentId = getId();
		if (detachFromParent())
			if (auto ptr = getDataModel()->takeStandaloneModel(this))
			{
				if (auto newParentObject = newParent->as<vl::Object>())
					newParentObject->setModel(newParentObject->getFreeId(currentId), ptr);
				else if (newParent->isList())
					newParent->as<vl::List>()->addModel(ptr);
				else
					return false;
				emit parentChanged();
				return true;
			}
		return false;
	}

	bool VLVarModel::removeChild(const VLVarModel *childPtr)
	{
		// Only objects and lists can use this method
		return false;
	}

	const VLVarModelPtr &VLVarModel::getChild(const VLVarModel *p) const
	{
		// Only objects and lists can use this method
		return nullVarModelPtr;
	}

	const VLVarModelPtr& VLVarModel::getPtr()
	{
		if (auto parent = getParentModel())
			return parent->getChild(this);
		else
			return nullVarModelPtr;
	}

	bool VLVarModel::remove()
	{
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
				if (parent->as<vl::Object>()->renameProperty(getId(), newId))
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

	bool VLVarModel::setType(const ObjectProperty::Type &newType)
	{
		if (auto parent = parentModel())
			return parent->setChildType(this, newType);
		return false;
	}

	const vl::Var &VLVarModel::setChildValue(const VLVarModel *child, const vl::VarPtr &value)
	{
		// Default logic
		return vl::emptyVar;
	}

	QVariant VLVarModel::value() const
	{
		auto& v = getData();
		if (v.is<vl::Object>())
			return QVariant::fromValue(const_cast<VLVarModel*>(this));
		else if (v.is<vl::Bool>())
			return QVariant(v.as<vl::Bool>().Val());
		else if (v.is<vl::String>())
		{
			LOCAL_VERBOSE(this << "->value() string requested '" << v.as<vl::String>().Val().c_str() << "'");
			return QVariant(v.as<vl::String>().Val().c_str());
		}
		else if (v.is<vl::Number>())
			return QVariant(v.as<vl::Number>().Val());
		else if (v.is<vl::List>())
			return QVariant::fromValue(const_cast<VLVarModel*>(this));
		else
			return QVariant(v.ToStr().c_str());
	}

	bool VLVarModel::setValue(const QVariant &newValue)
	{
		// Set through the parent to let the vl object propogate events
		if (auto parent = getParentModel())
			parent->setChildValue(this, ObjectProperty::makeVarFromData(newValue, getData().GetType()));
		else
		{
			auto& v = data();
			if (v.is<vl::String>())
				v.as<vl::String>() = newValue.toString().toStdString();
			else if (v.is<vl::Number>())
				v.as<vl::Number>() = newValue.toInt();
			else if (v.is<vl::Bool>())
				v.as<vl::Bool>() = newValue.toBool();
			emit valueChanged();
		}
		return true;
	}

	const std::string &VLVarModel::getId() const
	{
		if (auto parent = getParentModel())
			if (parent->isObject())
				if (auto id = parent->as<vl::Object>()->getChildId(this))
					return *id;
		return emptyString;
	}

	std::string VLVarModel::getStringId() const
	{
		if (auto parent = getParentModel())
		{
			if (parent->isObject())
			{
				if (auto id = parent->as<vl::Object>()->getChildId(this))
					return *id;
			}
			else if (parent->isList())
				return utils::format_str("[%d]", parent->as<vl::List>()->getChildIndex(this));
		}
		return emptyString;
	}

	int VLVarModel::getIndex() const
	{
		if (auto parent = getParentModel())
			if (parent->isList())
				return parent->as<vl::List>()->getChildIndex(this);
		return -1;
	}

	QVariant VLVarModel::valueStr() const
	{
		return QVariant(getData().ToStr().c_str());
	}
}
