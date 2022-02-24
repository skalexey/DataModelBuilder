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
	{}

	VLListVarModel::~VLListVarModel()
	{}

	void VLListVarModel::Init(QObject *parent, const vl::Var& data, DMBModel* owner)
	{
		Base::Init(parent, data, owner);
		mListModel.Init(shared_from_this());
	}

	VLListVarModel::VLListVarModel(const vl::Var& v, QObject* parent)
		: Base(v, parent)
	{}

	void VLListVarModel::Init(const VLVarModelPtr &parent)
	{
		Base::Init(parent);
		mListModel.Init(shared_from_this());
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
		return Base::getData().AsList();
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
		return mListModel.getDataAt(getChildIndex(childPtr));
	}

	int VLListVarModel::getChildIndex(const VLVarModel *childPtr) const
	{
		return mListModel.getElementIndex(childPtr);
	}

	vl::List &VLListVarModel::data()
	{
		return const_cast<vl::List&>(const_cast<const VLListVarModel*>(this)->getData());
	}

	const VLVarModel *VLListVarModel::getAt(int index) const
	{
		return mListModel.getAt(index);
	}

	dmb::VLVarModel *VLListVarModel::at(int index)
	{
		return mListModel.at(index);
	}

	const VLVarModelPtr &VLListVarModel::getAtSp(int index) const
	{
		return mListModel.getAtSp(index);
	}

	const VLVarModelPtr &VLListVarModel::atSp(int index)
	{
		return mListModel.atSp(index);
	}

	VLVarModel* VLListVarModel::add(ObjectProperty::Type type, int indexBefore)
	{
		return mListModel.add(type, indexBefore);
	}

	VLVarModel* VLListVarModel::add(const QVariant &data, int indexBefore)
	{
		return mListModel.add(data, indexBefore);
	}

	VLVarModel *VLListVarModel::add(const VLVarModel *model, int indexBefore)
	{
		return mListModel.add(model, indexBefore);
	}

	const VLVarModelPtr &VLListVarModel::addModel(const VLVarModelPtr &modelPtr, int indexBefore)
	{
		return mListModel.addModel(modelPtr, indexBefore);
	}

	bool VLListVarModel::removeChild(const VLVarModel *childPtr)
	{
		return removeAt(getChildIndex(childPtr));
	}

	const VLVarModelPtr &VLListVarModel::getChildPtr(const VLVarModel *p) const
	{
		return getAtSp(getChildIndex(p));
	}

	VLVarModelPtr VLListVarModel::getPtr()
	{
		if (auto ptr = Base::getPtr())
			return ptr;
		return std::dynamic_pointer_cast<VLVarModel>(shared_from_this());
	}

	bool VLListVarModel::removeAt(int index)
	{
		return mListModel.removeAt(index);
	}

	VLVarModel *VLListVarModel::find(const QVariant &data)
	{
		return mListModel.find(data);
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
		return mListModel.dataSize();
	}

	const VLListModel *VLListVarModel::getListModel() const
	{
		return &mListModel;
	}

	VLListModel *VLListVarModel::listModel()
	{
		if (!mListModel.elementsLoaded())
			loadElements();
		return &mListModel;
	}

	bool VLListVarModel::loadElements()
	{
//		qDebug() << "Load element models of list '" << getId().c_str() << "' ";
		bool result = mListModel.loadElementModels();
//		if (result)
//			qDebug() << " loaded\n";
//		else
//			qDebug() << " not loaded\n";
		return result;
	}
}
