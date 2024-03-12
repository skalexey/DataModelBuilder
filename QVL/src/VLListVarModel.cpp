#include <QObject>
#include "VLListVarModel.h"
#include "VLCollectionModel.h"
#include "vl.h"
#include "VLListModel.h"
#include "VLVarModel.h"
#include "VarModelFactory.h"
#include "DMBModel.h"

namespace dmb
{
	VLListVarModel::VLListVarModel(QObject *parent)
		: Base(parent)
		, mListModel(std::make_unique<VLListModel>(this, std::make_shared<ListModelStorage>()))
	{}

	VLListVarModel::VLListVarModel(const vl::Var& v, QObject* parent)
		: Base(v, parent)
		, mListModel(std::make_unique<VLListModel>(this, std::make_shared<ListModelStorage>()))
	{}

	VLListVarModel::~VLListVarModel()
	{
		qDebug() << "~VLListVarModel() " << this;
		emit beforeRemove();
		mListModel.reset();
	}

	void VLListVarModel::Init(QObject *parent, DMBModel* owner)
	{
		Base::Init(parent, owner);
		getListModel().Init(shared_from_this());
	}

	void VLListVarModel::Init(const VLVarModelPtr &parent)
	{
		Base::Init(parent);
		getListModel().Init(shared_from_this());
	}

	void VLListVarModel::Init(QObject *parent)
	{
		Base::Init(parent);
	}

	const VLListVarModel *VLListVarModel::asList() const
	{
		return this;
	}

	VLListVarModel *VLListVarModel::asList()
	{
		return this;
	}

	bool VLListVarModel::isList() const
	{
		return true;
	}

	const vl::List &VLListVarModel::getData() const
	{
		return Base::getData().as<vl::List>();
	}

	const vl::Var &VLListVarModel::getData(int index) const
	{
		return getData().At(index);
	}

	vl::Var &VLListVarModel::data(int index)
	{
		return const_cast<vl::Var&>(const_cast<const VLListVarModel*>(this)->getData(index));
	}

	const vl::Var &VLListVarModel::getChildData(const VLVarModel *childPtr) const
	{
		return getListModel().getData(getChildIndex(childPtr));
	}

	int VLListVarModel::getChildIndex(const VLVarModel *childPtr) const
	{
		return getListModel().getElementIndex(childPtr);
	}

	vl::List &VLListVarModel::data()
	{
		return const_cast<vl::List&>(const_cast<const VLListVarModel*>(this)->getData());
	}

	QVariant VLListVarModel::at(int index)
	{
		return QVariant::fromValue(getListModel().at(index));
	}

	const VLVarModelPtr &VLListVarModel::getModelAt(int index) const
	{
		return getListModel().getModelAt(index);
	}

	const VLVarModelPtr &VLListVarModel::modelAt(int index)
	{
		return getListModel().modelAt(index);
	}

	QVariant VLListVarModel::add(ObjectProperty::Type type, int indexBefore)
	{
		return getListModel().add(type, indexBefore);
	}

	QVariant VLListVarModel::add(const QVariant &data, int indexBefore)
	{
		return getListModel().add(data, indexBefore);
	}

	QVariant VLListVarModel::add(VLVarModel *model, int indexBefore)
	{
		return getListModel().add(model, indexBefore);
	}

	const VLVarModelPtr &VLListVarModel::addModel(const VLVarModelPtr &modelPtr, int indexBefore)
	{
		return getListModel().addModel(modelPtr, indexBefore);
	}

	bool VLListVarModel::removeChild(const VLVarModel *childPtr)
	{
		return removeAt(getChildIndex(childPtr));
	}

	const VLVarModelPtr &VLListVarModel::getChild(const VLVarModel *p) const
	{
		return getModelAt(getChildIndex(p));
	}

	const vl::Var &VLListVarModel::setChildValue(const VLVarModel *child, const vl::VarPtr &value)
	{
		return getListModel().setElementValue(child, value);
	}

	bool VLListVarModel::removeAt(int index)
	{
		return getListModel().removeAt(index);
	}

	QVariant VLListVarModel::find(const QVariant &data)
	{
		return getListModel().find(data);
	}

	void VLListVarModel::instantiate(const QString &typeId)
	{
		if (auto owner = getDataModel())
		{
			auto o = owner->createObjectSp();
			if (o->setPrototype(typeId))
			{
				addModel(o);
				emit instantiated(typeId);
			}
			else
				emit instantiateRefused("Type error");
		}
	}

	int VLListVarModel::size() const
	{
		return getListModel().dataSize();
	}

	QVariant VLListVarModel::listModel()
	{
		if (!getListModel().elementsLoaded())
			loadElements();
		return QVariant::fromValue(&getListModel());
	}

	bool VLListVarModel::loadElements()
	{
//		qDebug() << "Load element models of list '" << getId().c_str() << "' ";
		bool result = getListModel().loadElementModels();
//		if (result)
//			qDebug() << " loaded\n";
//		else
//			qDebug() << " not loaded\n";
		return result;
	}
}
