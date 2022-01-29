#include <QObject>
#include "VLListVarModel.h"
#include "VLCollectionModel.h"
#include "vl.h"
#include "VLListModel.h"
#include "VLVarModel.h"
#include "VarModelFactory.h"

namespace dmb
{
	VLListVarModel::VLListVarModel(QObject *parent)
		: Base(parent)
	{}

	VLListVarModel::~VLListVarModel()
	{}

	void VLListVarModel::Init(QObject *parent, const vl::Var& data)
	{
		Base::Init(parent, data);
		mListModel.Init(shared_from_this());
	}

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

	vl::Var &VLListVarModel::getData(int index)
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

	vl::List &VLListVarModel::getData()
	{
		return const_cast<vl::List&>(const_cast<const VLListVarModel*>(this)->getData());
	}

	const VLVarModel *VLListVarModel::getAt(int index) const
	{
		return mListModel.getAt(index);
	}

	VLVarModel *VLListVarModel::getAt(int index)
	{
		return mListModel.at(index);
	}

	void VLListVarModel::add(ObjectProperty::Type type)
	{
		mListModel.add(type);
	}

	bool VLListVarModel::removeAt(int index)
	{
		return mListModel.removeAt(index);
	}

	QObject *VLListVarModel::at(int index)
	{
		return getAt(index);
	}

	const VLListModel *VLListVarModel::listModel() const
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
