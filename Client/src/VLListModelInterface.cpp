#include <limits>
#include <QVariantList>
#include <QVector>
#include "VLListModelInterface.h"
#include "Utils.h"
#include "vl.h"
#include "VarModelFactory.h"
#include "VLVarModel.h"
#include "VLObjectVarModel.h"
#include "VLListVarModel.h"
#include "DMBModel.h"

namespace dmb
{
	QVector<int> emptyList;

	VLListModelInterface::VLListModelInterface(QObject* parent)
		: QAbstractListModel(parent)
	{}

	VLListModelInterface::~VLListModelInterface()
	{
		setParent(nullptr);
	}

	void VLListModelInterface::Init(QObject* parent)
	{
		setParent(parent);
	}

	void VLListModelInterface::Init(const VLVarModelPtr& parentModel)
	{
		mParentModel = parentModel;
	}

	bool VLListModelInterface::elementsLoaded() const
	{
		return mStorage.size() >= dataSize();
	}

	bool VLListModelInterface::loadElementModels()
	{
		auto sz = dataSize();
		mStorage.resize(sz);
		for (int i = 0; i < sz; i++)
			if (!loadElementModel(i, false))
				return false;
		return true;
	}

	VLVarModelPtr VLListModelInterface::loadElementModel(int index, bool resize)
	{
		if (resize)
			if (mStorage.size() <= index)
				mStorage.resize(index + 1);
		VLVarModelPtr ptr(nullptr);
		if (auto parent = getParentModel())
		{
			if (parent->isObject())
			{
				auto& id = parent->asObject()->getId(index);
				//qDebug() << "Create model for '" << id.c_str() << "'";
				if (id == "proto")
				{
					auto& protoData = getDataAt(index).AsObject();
					if (auto dmbModel = parent->getDataModel())
					{
						auto protoId = dmbModel->getDataModel().GetTypeId(protoData);
						ptr = dmbModel->getTypesModel()->getModelSp(protoId);
						mStorage.put(index, ptr);
					}
					else
					{
						// Any element in the hierarchy should have a link to the model
						assert(false);
					}
				}
			}
			if (!ptr)
			{
				// Container should be prepared to fit in the range
				ptr = VarModelFactory::Instance().CreateEmpty(getDataAt(index));
				mStorage.put(index, ptr);
				ptr->Init(parent);
			}
		}
		else
		{
			// Parent should exist
			assert(false);
		}
		if (ptr)
		{
			//mStorage.put(index, ptr);
			QObject::connect(ptr.get(), SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
		}
		return ptr;
	}

	// ======= Begin of Public interface =======
	vl::Var &VLListModelInterface::getDataAt(int index)
	{
		return const_cast<vl::Var&>(const_cast<const VLListModelInterface*>(this)->getDataAt(index));
	}

	vl::Var& VLListModelInterface::setDataAt(int index)
	{
		return setDataAt(index, MakePtr(vl::NullVar()));
	}

	vl::Var& VLListModelInterface::setDataAt(int index, const vl::Var& value)
	{
		return setDataAt(index, MakePtr(value));
	}

	QVariant VLListModelInterface::roleValueStr(const VLVarModel* m, int index) const
	{
		if (auto model = getAt(index))
			return model->valueStr();
		return QVariant();
	}

	QVariant VLListModelInterface::roleValue(const VLVarModel* m, int index) const
	{
		return m->value();
	}

	QVariant VLListModelInterface::roleTypeStr(const VLVarModel* m, int index) const
	{
		return QVariant(QString(ObjectProperty::GetTypeStr(m->getData())));
	}

	QVariant VLListModelInterface::roleType(const VLVarModel* m, int index) const
	{
		return QVariant(ObjectProperty::ConvertVLType(m->getData()));
	}

	int VLListModelInterface::size() const
	{
		return mStorage.size();
	}

	void VLListModelInterface::clear()
	{
		mStorage.clear();
	}

	QVariant VLListModelInterface::role(const VLVarModel* m, int index, int role) const
	{
		switch (role) {
			case RoleId:
				return QVariant(m->id());
			case RoleType:
				return roleType(m, index);
			case RoleTypeStr:
				return roleTypeStr(m, index);
			case RoleValue:
				return roleValue(m, index);
			case RoleValueStr:
				return roleValueStr(m, index);
			case RoleParent:
				if (auto parentModel = getParentModel())
					return QVariant::fromValue(parentModel.get());
				else
					return QVariant(); // Null
		}
		return QVariant();
	}

	void VLListModelInterface::resetList(const std::function<void()>& doWithList)
	{
		beginResetModel();
		if (doWithList)
			doWithList();
		else
			clear();
		endResetModel();
	}

	VLVarModel *VLListModelInterface::at(int index)
	{
		if (index < 0)
			return nullptr;
		else if (index >= size())
			return loadElementModel(index).get();
		else if (auto& ptr = mStorage.at(index))
			return ptr.get();
		else
			return loadElementModel(index).get();
	}

	const VLVarModelPtr& VLListModelInterface::getAtSp(int index) const
	{
		if (index < 0 || index >= size())
			return nullVarModelPtr;
		return mStorage[index];
	}

	const VLVarModel *VLListModelInterface::getAt(int index) const
	{
		return getAtSp(index).get();
	}

	const VLVarModelPtr VLListModelInterface::getParentModel() const
	{
		return mParentModel.lock();
	}
	VLVarModelPtr VLListModelInterface::getParentModel()
	{
		return mParentModel.lock();
	}

	int VLListModelInterface::getElementIndex(const VLVarModel *elementPtr) const
	{
		return mStorage.getIndex(elementPtr);
	}

	bool VLListModelInterface::foreachElement(const std::function<bool (int, const VLVarModelPtr &)> &pred) const
	{
		auto sz = size();
		for (int i = 0; i < sz; i++)
			if (!pred(i, getAtSp(i)))
				return false;
		return true;
	}

	bool VLListModelInterface::foreachElement(const std::function<bool (int, const VLVarModelPtr &)> &pred)
	{
		// TODO: optimize performance
		auto pred2 = [&](int i, const VLVarModelPtr& v) {
			if (!pred(i, const_cast<const VLVarModelPtr&>(v)))
				return false;
			return true;
		};
		return const_cast<const VLListModelInterface*>(this)->foreachElement(pred2);
	}

	bool VLListModelInterface::removeAt(int index)
	{
		if (index >= 0 && index < size())
		{
			beginRemoveRows(QModelIndex(), index, index);
			bool result = doRemove(index);
			mStorage.remove(index);
			endRemoveRows();
			return result;
		}
		return false;
	}

	void VLListModelInterface::onValueChanged(int i)
	{
		QModelIndex index = this->index(i, 0, QModelIndex());
		emit dataChanged(index, index, QVector<int>() << RoleValue);
	}

	void VLListModelInterface::onTypeChanged(int i)
	{
		QModelIndex index = this->index(i, 0, QModelIndex());
		emit dataChanged(index, index, QVector<int>() << RoleType << RoleTypeStr << RoleValue);
	}
	// ======= End of Public interface =======

	// ====== Begin of QAbstractListModel interface ======
	int VLListModelInterface::rowCount(const QModelIndex &parent) const
	{
		// For list models only the root node (an invalid parent) should return the list's size. For all
		// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
		if (parent.isValid())
			return 0;
		return size();
	}

	QVariant VLListModelInterface::data(const QModelIndex &index, int role) const
	{
		int intIndex = index.row();
		if (index.isValid() && size() > intIndex)
			if (auto m = getAt(intIndex))
				return this->role(m, intIndex, role);
		return QVariant();
	}

	bool VLListModelInterface::setType(int index, ObjectProperty::Type type)
	{
		auto& v = getDataAt(index);
		if (ObjectProperty::ConvertVLType(v) != type)
			return setDataAt(index, ObjectProperty::MakeVarPtr(type));
		return false;
	}

	bool VLListModelInterface::doSetData(int index, const QVariant& value, int role)
	{
		switch (role) {
			case RoleType:
				// Emit dataChanged signal
				return setType(index, qvariant_cast<ObjectProperty::Type>(value));
			case RoleValue:
				// Should exist as we have checked the index range in setData
				// Emit dataChanged signal
				return at(index)->setValue(value);
		}
		return false;
	}

	bool VLListModelInterface::setData(const QModelIndex &index, const QVariant &value, int role)
	{
		int intIndex = index.row();
		if (intIndex >= size())
			return false;
		if (data(index, role) != value)
			if (doSetData(intIndex, value, role))
				return true;
		return false;
	}

	Qt::ItemFlags VLListModelInterface::flags(const QModelIndex &index) const
	{
		if (!index.isValid())
			return Qt::NoItemFlags;
		return Qt::ItemIsEditable; // FIXME: Implement me!
	}

	QHash<int, QByteArray> VLListModelInterface::roleNames() const
	{
		QHash<int, QByteArray> roles;
		roles[RoleId] = "id";
		roles[RoleType] = "type";
		roles[RoleTypeStr] = "typeStr";
		roles[RoleValue] = "value";
		roles[RoleValueStr] = "valueStr";
		roles[RoleParent] = "parentModel";
		return roles;
	}

	void VLListModelInterface::putModel(int index, const VLVarModelPtr &ptr)
	{
		if (mStorage.size() <= index)
			mStorage.resize(index + 1);
		mStorage.put(index, ptr);
	}

	int VLListModelInterface::MStorage::getIndex(const VLVarModel *e) const
	{
		auto it = mElementIndex.find(e);
		if (it != mElementIndex.end())
			return it->second;
		return -1;
	}

	int VLListModelInterface::MStorage::size() const
	{
		return mElements.size();
	}

	void VLListModelInterface::MStorage::resize(int newSize)
	{
		mElements.resize(newSize);
	}

	void VLListModelInterface::MStorage::clear()
	{
		mElements.clear();
		mElementIndex.clear();
	}

	void VLListModelInterface::MStorage::put(int index, const VLVarModelPtr& ptr)
	{
		mElements[index] = ptr;
		mElementIndex[ptr.get()] = index;
	}

	const VLVarModelPtr &VLListModelInterface::MStorage::operator[](int index) const
	{
		return mElements[index];
	}

	VLVarModelPtr &VLListModelInterface::MStorage::at(int index)
	{
		return mElements[index];
	}

	void VLListModelInterface::MStorage::remove(int index)
	{
		auto e = mElements[index].get();
		mElementIndex.erase(e);
		mElements.erase(mElements.begin() + index);
	}

	// ====== End of QAbstractListModel interface ======
}
