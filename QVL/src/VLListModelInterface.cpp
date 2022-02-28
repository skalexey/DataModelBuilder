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
#include "Log.h"

namespace {
	dmb::VLVarModelPtr emptyVarModelPtr;
}

namespace dmb
{
	QVector<int> emptyList;

	VLListModelInterface::VLListModelInterface(QObject* parent)
		: QAbstractListModel(parent)
	{}

	VLListModelInterface::~VLListModelInterface()
	{
		setParent(nullptr);
		auto sz = mStorage.size();
		for (int i = 0; i < sz; i++)
			if (auto ptr = mStorage.at(i))
				emit ptr->beforeRemove();
	}

	bool VLListModelInterface::Init(QObject* p)
	{
		if (parent() == p)
			return false;
		setParent(p);
		return true;
	}

	bool VLListModelInterface::Init(const VLVarModelPtr& parentModel)
	{
		if (getParentModel() == parentModel)
			return false;
		mParentModel = parentModel;
		return true;
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
			if (!loadElementModel(i))
				return false;
		return true;
	}

	const VLVarModelPtr& VLListModelInterface::loadElementModel(int index, int indexBefore)
	{
		if (index >= dataSize())
			return emptyVarModelPtr;
		bool complete = false;
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
						if (auto& p = dmbModel->modelByTypeId(protoId))
						{
							auto& ptr = mStorage.put(index, p, indexBefore);
							connectSignals(ptr.get());
							return ptr;
						}
						else
						{
							qDebug() << Utils::FormatStr("Can't find or create a model for proto '%s'", protoId.c_str()).c_str();
						}
					}
					else
					{
						// Any element in the hierarchy should have a link to the model
						assert(false);
					}
				}
			}
			if (!complete)
			{
				// Container should be prepared to fit in the range
				auto& ptr = mStorage.put(index
							 , VarModelFactory::Instance().CreateEmpty(getDataAt(index))
							 , indexBefore);
				ptr->Init(parent);
				connectSignals(ptr.get());
				return ptr;
			}
		}
		else
		{
			// Parent should exist
			assert(false);
		}
		return emptyVarModelPtr;
	}

	void VLListModelInterface::connectSignals(VLVarModel* model) const
	{
		QObject::connect(model, SIGNAL(idChanged(int)), this, SLOT(onNameChanged(int)));
		QObject::connect(model, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
		QObject::connect(model, SIGNAL(typeChanged(int)), this, SLOT(onTypeChanged(int)));
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

	void VLListModelInterface::clearAndNotify()
	{
		auto sz = size();
		if (sz == 0)
			return;
		beginRemoveRows(QModelIndex(), 0, sz - 1);
		clear();
		endRemoveRows();
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
		return atSp(index).get();
	}

	const VLVarModelPtr& VLListModelInterface::getAtSp(int index) const
	{
		if (index < 0 || index >= size())
			return nullVarModelPtr;
		return mStorage[index];
	}

	const VLVarModelPtr& VLListModelInterface::atSp(int index)
	{
		if (index < 0)
			return nullVarModelPtr;
		else if (index >= size())
			return loadElementModel(index);
		else if (auto& ptr = mStorage.at(index))
			return ptr;
		else
			return loadElementModel(index);
	}

	const VLVarModelPtr &VLListModelInterface::setAt(int index, const VLVarModelPtr &modelPtr)
	{
		if (index < 0 || index >= size())
			mStorage.resize(index + 1);
		setDataAt(index, vl::MakePtr(modelPtr->getData()), [&](bool) {
			return modelPtr;
		});
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

	bool VLListModelInterface::foreachElement(const std::function<bool (int, const VLVarModelPtr &)> &pred, bool recursive) const
	{
		auto sz = size();
		for (int i = 0; i < sz; i++)
			if (auto m = getAtSp(i))
			{
				if (recursive)
				{
					if (auto o = m->asObject())
						o->getPropListModel().foreachElement(pred, recursive);
					if (auto l = m->asList())
						l->getListModel().foreachElement(pred, recursive);
				}
				if (!pred(i, getAtSp(i)))
					return false;

			}
		return true;
	}

	bool VLListModelInterface::foreachElement(const std::function<bool (int, const VLVarModelPtr &)> &pred, bool recursive)
	{
		// TODO: optimize performance
		auto pred2 = [&](int i, const VLVarModelPtr& v) {
			if (!pred(i, const_cast<const VLVarModelPtr&>(v)))
				return false;
			return true;
		};
		return const_cast<const VLListModelInterface*>(this)->foreachElement(pred2, recursive);
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

	void VLListModelInterface::onNameChanged(int i)
	{
		// Only objects can use this
	}

	void VLListModelInterface::onValueChanged(int i)
	{
		QModelIndex index = this->index(i, 0, QModelIndex());
		emit dataChanged(index, index, QVector<int>() << RoleValue << RoleValueStr);
	}

	void VLListModelInterface::onTypeChanged(int i)
	{
		QModelIndex index = this->index(i, 0, QModelIndex());
		emit dataChanged(index, index, QVector<int>() << RoleType << RoleTypeStr);
		if (auto model = getAt(i))
			emit model->valueChanged(i);
	}

	void VLListModelInterface::onModelChanged(int i, int last)
	{
		auto roles = roleNames();
		QVector<int> rolesList;
		QHashIterator<int, QByteArray> it(roles);
		while (it.hasNext()) {
			it.next();
			rolesList << it.key();
		}
		QModelIndex index = this->index(i, 0, QModelIndex());
		if (last < 0)
			emit dataChanged(index, index, rolesList);
		else
		{
			QModelIndex indexLast = this->index(last, 0, QModelIndex());
			emit dataChanged(index, indexLast, rolesList);
		}
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
		{
			if (auto parentModel = getParentModel())
			{
				auto newDataPtr = ObjectProperty::MakeVarPtr(type);
				auto newModel = VarModelFactory::Instance().Create(*newDataPtr);
				connectSignals(newModel.get());
				return setDataAt(index, newDataPtr, [&] (bool modelAlreadyExists) {
					newModel->Init(parentModel);
					return newModel;
				});
			}
		}
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

	const VLVarModelPtr& VLListModelInterface::putModel(int index, const VLVarModelPtr &ptr, int indexBefore)
	{
		return mStorage.put(index, ptr, indexBefore);
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

	const VLVarModelPtr& VLListModelInterface::MStorage::put(int index, const VLVarModelPtr& ptr, int indexBefore)
	{
		if (!ptr)
			qDebug() << "Warning! An attemption to put nullptr in the container chilren models storage";

		auto sz = mElements.size();
		if (indexBefore >= 0)
		{
			for (auto& [p, i] : mElementIndex)
				if (i >= index)
					i++;
			mElements.resize(sz + 1);
			sz++;
			for (int i = sz - 2; i >= index && i >= 0; i--)
				mElements[i + 1] = mElements[i];
		}
		else if (index >= sz)
			resize(index + 1);
		auto& result = mElements[index] = ptr;
		mElementIndex[ptr.get()] = index;
		return result;
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
		for (auto& [_, i] : mElementIndex)
			if (i > index)
				i--;
	}

	dmb::VLVarModel *VLListModelInterface::getParentModelProp() const
	{
		return getParentModel().get();
	}

	// ====== End of QAbstractListModel interface ======
}
