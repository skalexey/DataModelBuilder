#include "VLListModel.h"
#include "VLListVarModel.h"
#include "DMBModel.h"

namespace
{
	dmb::VLVarModelPtr emptyModelPtr = nullptr;
}

namespace dmb
{
	VLListModel::VLListModel(QObject *parent)
		: Base(parent)
	{}

	VLListVarModelPtr VLListModel::getParentModel() const
	{
		if (auto parentModel = Base::getParentModel())
			return std::dynamic_pointer_cast<VLListVarModel>(parentModel);
		return nullptr;
	}

	int VLListModel::dataSize() const
	{
		if (const auto* parentModel = getParentModel().get())
			return parentModel->getData().Size();
		return 0;
	}

	// ======= Begin of VLListModelInterface interface =======
	const vl::List &VLListModel::getData() const
	{
		if (const auto* parentModel = getParentModel().get())
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

	vl::Var &VLListModel::setDataAt(int index, const vl::VarPtr &ptr, const std::function<VLVarModelPtr(bool alreadyExist)>& customModelLoader)
	{
		auto& result = getData().Set(index, ptr);
		if (customModelLoader)
			putModel(index, customModelLoader(false));
		return result;
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

	VLVarModel* VLListModel::add(ObjectProperty::Type type, int indexBefore)
	{
		if (addData(type), indexBefore)
			return at(size() - 1);
		return nullptr;
	}

	VLVarModel *VLListModel::add(const QVariant &data, int indexBefore)
	{
		if (data.canConvert<VLVarModel*>())
			return add(qvariant_cast<VLVarModel*>(data), indexBefore);
		if (auto parent = getParentModel())
			if (auto dataModel = parent->getDataModel())
				return add(dataModel->createFromData(data), indexBefore);
		return nullptr;
	}

	VLVarModel *VLListModel::add(const VLVarModel *m, int indexBefore)
	{
		if (auto parentModel = getParentModel())
			if (auto dataModel = parentModel->getDataModel())
				if (auto modelPtr = dataModel->takeStandaloneModel(m))
					return addModel(modelPtr, indexBefore).get();
		return nullptr;
	}

	const VLVarModelPtr& VLListModel::addModel(const VLVarModelPtr& modelPtr, int indexBefore)
	{
		if (auto parentModel = getParentModel()) {
			if (auto dataModel = parentModel->getDataModel())
			{
				auto standaloneModelPtr = dataModel->takeStandaloneModel(modelPtr.get());
				if (!standaloneModelPtr) // Already taken
					standaloneModelPtr = modelPtr;
				const VLVarModel* m = standaloneModelPtr.get();
				int index = -1;
				addData(vl::MakePtr(m->getData()), indexBefore, [&] (int newIndex) {
					index = newIndex;
					putModel(index, standaloneModelPtr, indexBefore);
					standaloneModelPtr->Init(parentModel);
				});
				return getAtSp(index);
			}
		}
		return emptyModelPtr;
	}

	vl::Var &VLListModel::addData(const vl::VarPtr &ptr, int indexBefore, const std::function<void(int newIndex)>& customModelLoader)
	{
		auto sz = size();
		QModelIndex index = this->index(sz, 0, QModelIndex());
		beginInsertRows(index, sz, sz);
		auto& result = getData().Add(ptr, indexBefore);
		auto newIndex = indexBefore >= 0 ? indexBefore : sz;
		if (customModelLoader)
			customModelLoader(newIndex);
		else
			loadElementModel(newIndex, indexBefore);
		endInsertRows();
		return result;
	}

	vl::Var& VLListModel::addData(ObjectProperty::Type type, int indexBefore)
	{
		return addData(ObjectProperty::MakeVarPtr(type), indexBefore);
	}

	vl::Var &VLListModel::addData(const vl::Var &v, int indexBefore)
	{
		return addData(vl::MakePtr(v), indexBefore);
	}

	VLVarModel *VLListModel::find(const QVariant &data)
	{
		VLVarModel* result = nullptr;
		foreachElement([&](int index, const VLVarModelPtr& model) {
			if (data.canConvert<VLVarModel*>())
			{
				if (model.get() == qvariant_cast<VLVarModel*>(data))
					result = model.get();
			}
			else
			{
				const VLVarModel* m = model.get();
				switch (data.userType())
				{
				case QMetaType::QString:
					if (m->getData().AsString().Val() == data.toString().toStdString())
						result = model.get();
					break;
				case QMetaType::Int:
				case QMetaType::Double:
				case QMetaType::Float:
					if (m->getData().AsNumber().Val() == data.toInt())
						result = model.get();
					break;
				case QMetaType::Bool:
					if (m->getData().AsBool().Val() == data.toBool())
						result = model.get();
				}
			}
			if (result != nullptr)
				return false;
			return true;
		});
		return result;
	}
}
