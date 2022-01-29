#include "VLListModel.h"
#include "VLListVarModel.h"

namespace dmb
{
	VLListModel::VLListModel(QObject *parent)
		: Base(parent)
	{}

	const VLListVarModel *VLListModel::getParentModel() const
	{
		if (auto parentModel = Base::getParentModel())
			return parentModel->asList();
		return nullptr;
	}

	VLListVarModel *VLListModel::getParentModel()
	{
		return const_cast<VLListVarModel*>(const_cast<const VLListModel*>(this)->getParentModel());
	}

	int VLListModel::dataSize() const
	{
		if (auto parentModel = getParentModel())
			return parentModel->getData().Size();
		return 0;
	}

	// ======= Begin of VLListModelInterface interface =======
	const vl::List &VLListModel::getData() const
	{
		if (auto parentModel = getParentModel())
			return parentModel->getData();
		return vl::emptyList;
	}

	vl::List &VLListModel::getData()
	{
		return const_cast<vl::List&>(const_cast<const VLListModel*>(this)->getData());
	}

	const vl::Var &VLListModel::getDataAt(int index) const
	{
		return getData().At(index);
	}

	vl::Var &VLListModel::setDataAt(int index, const vl::VarPtr &ptr)
	{
		return getData().Set(index, ptr);
	}

	bool VLListModel::doRemove(int index)
	{
		return getData().Remove(index);
	}
	// ======= End of VLListModelInterface interface =======

	QHash<int,QByteArray> VLListModel::roleNames() const
	{
		QHash<int, QByteArray> roles = Base::roleNames();
		return roles;
	}

	bool VLListModel::add(ObjectProperty::Type type)
	{
		return addData(type);
	}

	vl::Var& VLListModel::addData(ObjectProperty::Type type)
	{
		auto sz = size();
		QModelIndex index = this->index(sz, 0, QModelIndex());
		beginInsertRows(index, sz, sz);
		auto& result = getData().Add(ObjectProperty::MakeVarPtr(type));
		loadElementModel(sz);
		endInsertRows();
		return result;
	}
}
