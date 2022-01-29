#include <string>
#include "VLVarModel.h"
#include "vl.h"
#include "VLObjectVarModel.h"

namespace dmb
{
	VLVarModelPtr nullVarModelPtr = nullptr;
	std::string emptyString;

	VLVarModel::VLVarModel(QObject *parent)
		: QObject(parent)
	{
//		qDebug() << "Create VarModel " << this;
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

	void VLVarModel::Init(QObject *parent, const vl::Var &data)
	{
		setParent(parent);
		mVarPtr = vl::MakePtr(data);
	}

	void VLVarModel::Init(const VLVarModelPtr& parent)
	{
		setParent(parent.get());
		mParentModel = parent;
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

	vl::Var &VLVarModel::getChildData(const VLVarModel *childPtr)
	{
		return const_cast<vl::Var&>(const_cast<const VLVarModel*>(this)->getChildData(childPtr));
	}

	const VLVarModel *VLVarModel::getParentModel() const
	{
		return mParentModel.lock().get();
	}

	VLVarModel *VLVarModel::getParentModel()
	{
		return mParentModel.lock().get();
	}

	int VLVarModel::getIndex() const
	{
		if (auto parent = getParentModel())
			return parent->getChildIndex(this);
		return -1;
	}

	int VLVarModel::getChildIndex(const VLVarModel *childPtr) const
	{
		// Only objects and lists can have children
		return -1;
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
		if (auto parent = getParentModel())
			if (parent->isObject())
				if (parent->asObject()->renameProperty(getId(), newId))
				{
					emit idChanged(getIndex());
					return true;
				}
		return false;
	}

	ObjectProperty::Type VLVarModel::type() const
	{
		return ObjectProperty::ConvertVLType(getData());
	}

	void VLVarModel::setType(const ObjectProperty::Type &newType)
	{
		if (auto parent = getParentModel())
		{
			if (parent->isObject())
			parent->asObject()->propListModel()->setData(getId(), ObjectProperty::MakeVarPtr(newType));
			emit typeChanged(getIndex());
		}
	}

	QVariant VLVarModel::value() const
	{
		auto& v = getData();
		if (v.IsObject())
			return QVariant::fromValue(this);
		else if (v.IsBool())
			return QVariant(v.AsBool().Val());
		else if (v.IsString())
			return QVariant(v.AsString().Val().c_str());
		else if (v.IsNumber())
			return QVariant(v.AsNumber().Val());
		else if (v.IsList())
			return QVariant::fromValue(this);
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
