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
#include "ListModelStorageSubscriptionProcessor.h"

#include "Log.h"
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("VLListModelInterface")
LOG_STREAM([]{return qDebug();})
SET_LOG_VERBOSE(false)

namespace {
	dmb::VLVarModelPtr emptyVarModelPtr;
}

namespace dmb
{
	QVector<int> emptyList;

	VLListModelInterface::VLListModelInterface(QObject *parent)
		: Base(parent)
	{}

	VLListModelInterface::VLListModelInterface(QObject* parent, const ModelStoragePtr& storage)
		: QAbstractListModel(parent)
	{
		assert(storage != nullptr);
		mStorage = storage;		
	}

	VLListModelInterface::VLListModelInterface(QObject *parentModel, const VLListModelInterface &storageOwner)
		: VLListModelInterface(parentModel, storageOwner.getSharedStorage())
	{
		if (auto parent = storageOwner.getParentModel())
			Init(parent);
	}

	VLListModelInterface::~VLListModelInterface()
	{
		if (mStorageSubscriptionProcessor)
		{
			if (mStorage)
				mStorage->Detach(mStorageSubscriptionProcessor.get());
			mStorageSubscriptionProcessor.reset();
		}
	}

	bool VLListModelInterface::Init(QObject* p)
	{
		if (parent() == p)
			return false;
		qDebug() << this << "->(listInterface)setParent(" << p << ")";
		setParent(p);
		return true;
	}

	bool VLListModelInterface::Init(const VLVarModelPtr& parentModel)
	{
		if (getParentModel() == parentModel)
			return false;
		mParentModel = parentModel;
		if ((mStorageSubscriptionProcessor = createStorageSubscriptionProcessor()))
			mStorage->Attach(mStorageSubscriptionProcessor.get());
		getStorage().Init(parentModel);
		Init(parentModel.get());
		return true;
	}

	// Common code which should be run on any storage holder
	// when a model is put into it
	void VLListModelInterface::onModelPut(const VLVarModelPtr& m)
	{
		connectSignals(m.get());
	}

	void VLListModelInterface::onModelBeforeRemove(const VLVarModelPtr& m)
	{
		disconnectSignals(m.get());
	}

	bool VLListModelInterface::elementsLoaded() const
	{
		return getStorage().size() >= dataSize();
	}

	void VLListModelInterface::connectSignals(VLVarModel* model) const
	{
		QObject::connect(model, SIGNAL(idChanged()), this, SLOT(onNameChanged()));
		QObject::connect(model, SIGNAL(valueChanged()), this, SLOT(onValueChanged()));
		QObject::connect(model, SIGNAL(typeChanged()), this, SLOT(onTypeChanged()));
	}

	void VLListModelInterface::disconnectSignals(VLVarModel *model) const
	{
		QObject::disconnect(model, SIGNAL(idChanged()), this, SLOT(onNameChanged()));
		QObject::disconnect(model, SIGNAL(valueChanged()), this, SLOT(onValueChanged()));
		QObject::disconnect(model, SIGNAL(typeChanged()), this, SLOT(onTypeChanged()));
	}
	// ======= Begin of Public interface =======
	QVariant VLListModelInterface::roleValueStr(const VLVarModelPtr& m, int index) const
	{
		return m->valueStr();
	}

	QVariant VLListModelInterface::roleValue(const VLVarModelPtr& m, int index) const
	{
		return m->value();
	}

	QVariant VLListModelInterface::roleTypeStr(const VLVarModelPtr& m, int index) const
	{
		return QVariant(QString(ObjectProperty::GetTypeStr(m->getData())));
	}

	QVariant VLListModelInterface::roleType(const VLVarModelPtr& m, int index) const
	{
		return QVariant(ObjectProperty::ConvertVLType(m->getData()));
	}

	int VLListModelInterface::size() const
	{
		return getStorage().size();
	}

	void VLListModelInterface::clear()
	{
		getStorage().clear();
	}

	// Not needed in subscribed approach
	void VLListModelInterface::clearAndNotify()
	{
		auto sz = size();
		if (sz == 0)
			return;
		beginRemoveRows(QModelIndex(), 0, sz - 1);
		clear();
		endRemoveRows();
	}

	QVariant VLListModelInterface::role(const VLVarModelPtr& m, int index, int role) const
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

	void VLListModelInterface::beginInsertNewRow()
	{
		auto sz = dataSize();
		QModelIndex index = this->index(sz, 0, QModelIndex());
		beginInsertRows(QModelIndex(), sz, sz);
	}

	// Q_INVOKABLE
	// Non-const version of getAt
	// Redirect to atSp
	// Retrieve a raw pointer from the storage to a model at index
	// Create a model if it doesn't exists
	QVariant VLListModelInterface::at(int index)
	{
		return QVariant::fromValue(modelAt(index).get());
	}

	bool VLListModelInterface::foreachElement(const std::function<bool (int, const VLVarModelPtr &)> &pred, bool recursive) const
	{
		auto sz = size();
		for (int i = 0; i < sz; i++)
			if (auto& m = getModelAt(i))
			{
				if (recursive)
				{
					if (auto o = m->asObject())
						o->getOwnPropsListModel().foreachElement(pred, recursive);
					if (auto l = m->asList())
						l->getListModel().foreachElement(pred, recursive);
				}
				if (!pred(i, getModelAt(i)))
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

	const vl::Var &VLListModelInterface::setElementValue(const VLVarModel *e, const vl::VarPtr &value)
	{
		// Default logic
		return vl::emptyVar;
	}

	bool VLListModelInterface::removeAt(int index)
	{
		if (index >= 0 && index < size())
		{
			beginRemoveRows(QModelIndex(), index, index);
			bool result = doRemove(index);
			if (auto& m = getStorage().at(index))
				onModelBeforeRemove(m);
			getStorage().remove(index);
			endRemoveRows();
			return result;
		}
		return false;
	}

	void VLListModelInterface::onNameChanged()
	{
		// Only objects can use this
	}

	void VLListModelInterface::onValueChanged()
	{
		auto sender = QObject::sender();
		auto i = getElementIndex(sender);
		if (i < 0)
		{
			LOG_ERROR("VLListModelInterface::onValueChanged slot received a signal from an unregistered sender " << QObject::sender());
			return;
		}
		QModelIndex index = this->index(i, 0, QModelIndex());
		emit dataChanged(index, index, QVector<int>() << RoleValue << RoleValueStr);
	}

	void VLListModelInterface::onTypeChanged()
	{
		auto sender = QObject::sender();
		auto i = getElementIndex(sender);
		if (i < 0)
		{
			LOG_ERROR("VLListModelInterface::onTypeChanged slot received a signal from an unregistered sender " << sender);
			return;
		}
		QModelIndex index = this->index(i, 0, QModelIndex());
		emit dataChanged(index, index, QVector<int>() << RoleType << RoleTypeStr);
		if (auto& model = getModelAt(i))
			emit model->valueChanged();
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
		auto sz = size();
		LOCAL_VERBOSE("rowCount: " << int(sz));
		return sz;
	}

	QVariant VLListModelInterface::data(const QModelIndex &index, int role) const
	{
		int intIndex = index.row();
		if (index.isValid() && size() > intIndex)
			if (auto& m = getModelAt(intIndex))
				return this->role(m, intIndex, role);
		return QVariant();
	}

	VLCollectionModel *VLListModelInterface::asCollection()
	{
		// Default logic
		return nullptr;
	}

	bool VLListModelInterface::isCollection() const
	{
		// Default logic
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

	dmb::VLVarModel *VLListModelInterface::getParentModelProp() const
	{
		return getParentModel().get();
	}

	std::unique_ptr<ModelStorageSubscriptionProcessor> VLListModelInterface::createStorageSubscriptionProcessor()
	{
		return std::make_unique<ListModelStorageSubscriptionProcessor>(*this);
	}

	// ====== End of QAbstractListModel interface ======
}
