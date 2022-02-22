#include <limits>
#include <QQmlApplicationEngine>
#include <QVariantList>
#include "VLCollectionModel.h"
#include "VLObjectVarModel.h"
#include "Utils.h"
#include "vl.h"
#include "DMBModel.h"
#include "Log.h"

namespace
{
	dmb::VLVarModelPtr emptyModelPtr;
}

namespace dmb
{
	VLCollectionModel::VLCollectionModel(QObject *parent)
		: Base(parent)
	{}

	void VLCollectionModel::Init(const VLVarModelPtr& objectPtr)
	{
		Base::Init(objectPtr);
		UpdateIdList();
	}

	void VLCollectionModel::UpdateIdList()
	{
		resetList([&]() {
			clear();
			getData().ForeachProp(
			[&](const std::string& propName, const vl::Var& prop) {
				mPropIndex[propName] = mIdList.size();
				mIdList.push_back(propName);
				return true;
			});
		});
	}

	int VLCollectionModel::dataSize() const
	{
		return getData().Size();
	}

	// ======= Begin of Base interface =======
	const vl::Var& VLCollectionModel::getDataAt(int index) const
	{
		if (index >= 0 && index < mIdList.size())
			return getData().Get(getId(index));
		return vl::emptyVar;
	}

	// overriden pure virtual Base::set(int index, const vl::VarPtr& ptr)
	vl::Var& VLCollectionModel::setDataAt(int index, const vl::VarPtr& ptr, const std::function<VLVarModelPtr(bool alreadyExist)>& customModelLoader)
	{
		return setData(getId(index), ptr, customModelLoader);
	}

	// Object-specific setters
	vl::Var& VLCollectionModel::setData(const std::string& propId)
	{
		return setData(propId, MakePtr(vl::NullVar()));
	}

	vl::Var& VLCollectionModel::setData(const std::string& propId, const vl::Var& value)
	{
		return setData(propId, MakePtr(value));
	}

	vl::Var& VLCollectionModel::setData(const std::string& propId, const vl::VarPtr& ptr, const std::function<VLVarModelPtr(bool alreadyExist)>& customModelLoader)
	{
		if (!hasData(propId))
		{
			auto sz = size();
			QModelIndex index = this->index(sz, 0, QModelIndex());
			beginInsertRows(index, sz, sz);
			auto& result = getData().Set(propId, ptr);
			mPropIndex[propId] = sz;
			mIdList.push_back(propId);
			if (customModelLoader)
				customModelLoader(false);
			else
				loadElementModel(sz);
			endInsertRows();
			return result;
		}
		else
		{
			auto& oldData = getData().Get(propId);
			bool newType = ObjectProperty::ConvertVLType(oldData) != ObjectProperty::ConvertVLType(*ptr);
			auto& result = getData().Set(propId, ptr);
			if (customModelLoader)
			{
				auto modelPtr = customModelLoader(true);
				putModel(getIndex(propId), modelPtr);
			}
			auto i = getIndex(propId);
			if (newType)
				if (auto model = getModel(propId))
					// Call VLListModelInterface::onValueChanged
					emit model->typeChanged(i);
			return result;
		}
	}


	void VLCollectionModel::clear()
	{
		Base::clear();
		mPropIndex.clear();
		mIdList.clear();
	}

	bool VLCollectionModel::doRemove(int index)
	{
		auto& id = getId(index);
		getData().RemoveProperty(id);
		mPropIndex.erase(id);
		for (auto& e : mPropIndex)
			if (e.second > index)
				e.second--;
		mIdList.erase(mIdList.begin() + index);
		return true;
	}

	QVariant VLCollectionModel::role(const VLVarModel* m, int index, int role) const
	{
		// Index already checked by the base functional
		switch (role) {
			case RoleName:
				// use internal name instead of model name
				// because the model name is the proto's object name
				return QVariant(getId(index).c_str());
			default:
				return Base::role(m, index, role);
		}
	}

	QVariant VLCollectionModel::roleValueStr(const VLVarModel* m, int index) const
	{
		// use internal name instead of model name
		// because the model name is the proto's object name
		if (getId(index) == "proto")
			return QVariant(
				Utils::FormatStr("{%s}"
				, m->getTypeId().c_str()).c_str()
			);
		else
			return Base::roleValueStr(m, index);
	}

	bool VLCollectionModel::renameElement(int index, const std::string& newId)
	{
		if (index >= 0 && index < mIdList.size())
			return renameElement(getId(index), newId);
		return false;
	}

	bool VLCollectionModel::renameElement(const std::string& propId, const std::string& newId)
	{
		auto it = mPropIndex.find(propId);
		if (it == mPropIndex.end())
			return false;
		auto index = it->second;
		getData().RenameProperty(propId, newId);
		mPropIndex.erase(it);
		mPropIndex[newId] = index;
		mIdList[index] = newId;
		if (auto model = getAt(index))
			// Call onNameChanged
			emit model->idChanged(index);
		return true;
	}

	bool VLCollectionModel::doSetData(int index, const QVariant& value, int role)
	{
		auto& id = getId(index);
		switch (role) {
			case RoleName:
				if (renameElement(index, value.toString().toStdString()))
					return true;
				else
					emit nameAlreadyTaken();
			default:
				return Base::doSetData(index, value, role);
		}
		return false;
	}

	bool VLCollectionModel::hasData(const std::string &propId) const
	{
		return getData().Has(propId);
	}
	// ======= End of Base interface =======
	// ====== Begin of QAbstractListModel interface ======
	QHash<int, QByteArray> VLCollectionModel::roleNames() const
	{
		QHash<int, QByteArray> roles = Base::roleNames();
		roles[RoleName] = "name";
		return roles;
	}

	const vl::Object &VLCollectionModel::getData() const
	{
		if (const auto* parentModel = getParentModel().get())
			return parentModel->getData();
		return vl::nullObject;
	}

	const vl::Var &VLCollectionModel::getData(const std::string &propId) const
	{
		return getData().Get(propId);
	}

	vl::Var &VLCollectionModel::getData(const std::string &propId)
	{
		return const_cast<vl::Var&>(const_cast<const VLCollectionModel*>(this)->getData(propId));
	}
	// ====== End of QAbstractListModel interface ======

	bool VLCollectionModel::addData(const std::string &name, ObjectProperty::Type type)
	{
		const auto& data = getData();
		auto id = name;
		bool success = true;
		if (data.Has(id)) {
			auto tpl = id;
			for (int i = 2; i < std::numeric_limits<short>::max(); i++)
				if ((success = !data.Has(id = Utils::FormatStr("%s %d", tpl.c_str(), i))))
					break;
		}
		if (success) {
			setData(id, ObjectProperty::MakeVarPtr(type));
			return true;
		}
		return false;
	}

	bool VLCollectionModel::add(const QString& propId, ObjectProperty::Type type)
	{
		return addData(propId.toStdString(), type);
	}

	bool VLCollectionModel::add(ObjectProperty::Type type)
	{
		return addData("New prop", type);
	}

	bool VLCollectionModel::remove(const std::string& propId)
	{
		return removeAt(getIndex(propId));
	}

	bool VLCollectionModel::remove(const QString &propId)
	{
		return remove(propId.toStdString());
	}

	void VLCollectionModel::onNameChanged(int i)
	{
		QModelIndex index = this->index(i, 0, QModelIndex());
		emit dataChanged(index, index, QVector<int>() << RoleName);
	}

	vl::Object &VLCollectionModel::getData()
	{
		return const_cast<vl::Object&>(const_cast<const VLCollectionModel*>(this)->getData());
	}

	VLObjectVarModelPtr VLCollectionModel::getParentModel() const
	{
		if (auto parentModel = Base::getParentModel())
			return std::dynamic_pointer_cast<VLObjectVarModel>(parentModel);
		return nullptr;
	}

	VLObjectVarModelPtr VLCollectionModel::getParentModel()
	{
		return const_cast<const VLCollectionModel*>(this)->getParentModel();
	}

	bool VLCollectionModel::has(const QString& propId) const
	{
		return has(propId.toStdString());
	}

	bool VLCollectionModel::has(const std::string &propId) const
	{
		return getData().Has(propId);
	}

	VLVarModel *VLCollectionModel::model(const std::string &propId)
	{
		//return const_cast<VLVarModel*>(const_cast<const VLCollectionModel*>(this)->getAt(getIndex(propId)));
		// Better to use at instead of const_cast

		if (auto m = at(getIndex(propId)))
			return m;
		if (propId == "proto")
			return nullptr;
		if (auto proto = at(getIndex("proto")))
			return proto->asObject()->model(propId);
		return nullptr;
	}

	const VLVarModelPtr &VLCollectionModel::setModel(const std::string &propId, const VLVarModelPtr &modelPtr)
	{
		const VLVarModel* m = modelPtr.get();
		auto dataPtr = vl::MakePtr(m->getData());
		auto parent = getParentModel();
		setData(propId, dataPtr, [&] (bool) {
			if (!modelPtr->getParentModel())
			{
				if (auto owner = modelPtr->getDataModel())
					if (auto standaloneModel = owner->takeStandaloneModel(modelPtr.get()))
					{
						putModel(getIndex(propId), standaloneModel);
						standaloneModel->Init(parent);
						return standaloneModel;
					}
				putModel(getIndex(propId), modelPtr);
				modelPtr->Init(parent);
				return modelPtr;
			}
			putModel(getIndex(propId), modelPtr);
			return modelPtr;
		});
		return getModelSp(propId);
	}

	const VLVarModel *VLCollectionModel::getModel(const std::string &propId) const
	{
		return getModelSp(propId).get();
	}

	const VLVarModelPtr& VLCollectionModel::getModelSp(const std::string &propId) const
	{
		if (auto& m = getAtSp(getIndex(propId)))
			return m;
		if (propId == "proto")
			return emptyModelPtr;
		if (auto proto = getModel("proto"))
			return proto->asObject()->getModelSp(propId);
		return emptyModelPtr;
	}

	const VLVarModelPtr& VLCollectionModel::modelSp(const std::string &propId)
	{
		if (auto& m = atSp(getIndex(propId)))
			return m;
		if (propId == "proto")
			return emptyModelPtr;
		if (auto proto = model("proto"))
			return proto->asObject()->modelSp(propId);
		return emptyModelPtr;
	}

	VLVarModel *VLCollectionModel::get(const QString& propId)
	{
		return modelSp(propId.toStdString()).get();
	}

	VLVarModel *VLCollectionModel::set(const QString &propId, VLVarModel *v)
	{
		if (auto parentModel = getParentModel()) {
			if (auto dataModel = parentModel->getDataModel())
			{
				const VLVarModel* m = v;
				auto id = propId.toStdString();
				auto dataPtr = vl::MakePtr(m->getData());
				if (auto modelPtr = dataModel->takeStandaloneModel(v))
				{
					setData(id, dataPtr, [&] (bool modelAlreadyExists) {
						modelPtr->Init(parentModel);
						putModel(getIndex(id), modelPtr);
						return modelPtr;
					});
				}
				return at(getIndex(id));
			}
		}
		return nullptr;
	}

	VLVarModel *VLCollectionModel::set(const QString &propId, const QVariant &data)
	{
		if (data.canConvert<VLVarModel*>())
			return set(propId, qvariant_cast<VLVarModel*>(data));
		if (auto parent = getParentModel())
			if (auto dataModel = parent->getDataModel())
				return set(propId, dataModel->createFromData(data));
		return nullptr;
	}

	const std::string &VLCollectionModel::getId(int index) const
	{
		// Don't check index as it should be checked on the above level
		return mIdList[index];
	}

	std::string& VLCollectionModel::getId(int index)
	{
		return const_cast<std::string&>(const_cast<const VLCollectionModel*>(this)->getId(index));
	}

	int VLCollectionModel::getIndex(const std::string &propId) const
	{
		auto it = mPropIndex.find(propId);
		if (it != mPropIndex.end())
			return it->second;
		return -1;
	}
}
