#include <limits>
#include <QQmlApplicationEngine>
#include <QVariantList>
#include <utils/string_utils.h>
#include <utils/Log.h>
#ifdef LOG_ON
    #include <QDebug>
#endif
#include "vl.h"
#include "VLCollectionModel.h"
#include "VLObjectVarModel.h"
#include "VarModelFactory.h"
#include "VLVarModel.h"
#include "DMBModel.h"
#include "ObjectModelStorageSubscriptionProcessor.h"
LOG_TITLE("VLCollectionModel")
LOG_STREAM([]{return qDebug();})
SET_LOG_VERBOSE(false)
SET_LOG_DEBUG(true)

namespace
{
	dmb::VLVarModelPtr nullVarModelPtr;
	const bool modelOverwriteRestricted = true;
}

namespace dmb
{
	VLCollectionModel::VLCollectionModel(QObject *parent)
		: Base(parent)
	{}

	VLCollectionModel::VLCollectionModel(QObject *parent, const ObjectModelStoragePtr& storage)
		: Base(parent, storage)
	{}

	VLCollectionModel::VLCollectionModel(QObject *parent, const VLListModelInterface &storageOwner)
		: Base(parent, storageOwner)
	{}

	VLCollectionModel::~VLCollectionModel()
	{
		LOCAL_DEBUG("~VLCollectionModel()");
		LOCAL_DEBUG(this << "->(collection)setParent(nullptr)");
		mVLSubscriptionProcessor.reset();
	}

	bool VLCollectionModel::Init(const VLVarModelPtr& newParent)
	{
		if (auto parent = getParentModel())
			if (parent != newParent)
				if (mVLSubscriptionProcessor)
					getData().AsObject().Detach(mVLSubscriptionProcessor.get());

		// If newParent is the same as own
		if (!Base::Init(newParent))
			return false;

		if (auto parent = getParentModel())
			if ((mVLSubscriptionProcessor = createVLSubacriptionProcessor()))
				getData().AsObject().Attach(mVLSubscriptionProcessor.get());
		return true;
	}

	std::unique_ptr<CollectionModelSubscriptionProcessor> VLCollectionModel::createVLSubacriptionProcessor()
	{
		// Default processor
		return std::make_unique<CollectionModelSubscriptionProcessor>(*this);
	}

	void VLCollectionModel::UpdateIdList()
	{
		resetList([&]() {
			getObjectStorage().idList().clear();
			getData().ForeachProp(
			[&](const std::string& propName, const vl::Var& prop) {
				getObjectStorage().idList().put(propName);
				return true;
			});
		});
	}

	int VLCollectionModel::dataSize() const
	{
		if (auto parent = getParentModel())
			return parent->sizeOwn();
		return 0;
	}

	// ======= Begin of Base interface =======
	// Work with indexes
	// Needed for VLListModelInterface
	// as it works with the list-like container
	const VLVarModelPtr &VLCollectionModel::setModelAt(int index, const VLVarModelPtr& ptr)
	{
		if (auto id = getId(index))
			return setModel(*id, ptr);
		LOCAL_ERROR("Can't find id at index '" << index << "' through the call " << "setModelAt(" << index << ", " << ptr.get() << ")");
		return nullVarModelPtr;
	}

	const VLVarModelPtr &VLCollectionModel::modelAt(int index)
	{
		if (index < 0 || index >= dataSize())
			return nullVarModelPtr;
		if (auto id = getId(index))
			return model(*id);
		return nullVarModelPtr;
	}

	// Needed for VLListModelInterface
	// as it works with the list-like container
	const VLVarModelPtr &VLCollectionModel::getModelAt(int index) const
	{
		if (auto id = getId(index))
			return getObjectStorage().get(*id);
		return nullVarModelPtr;
	}

	bool VLCollectionModel::doRemove(int index)
	{
		if (auto id = getId(index))
			return getData().RemoveProperty(*id);
		return false;
	}

	bool VLCollectionModel::doSetData(int index, const QVariant& value, int role)
	{
		auto& id = *getId(index);
		switch (role) {
			case RoleType:
				// Emit dataChanged signal
				return setType(id, qvariant_cast<ObjectProperty::Type>(value));
			case RoleName:
				if (renameElement(id, value.toString().toStdString()))
					return true;
				else
					emit nameAlreadyTaken();
			case RoleValue:
				// Emit dataChanged signal
				auto& data = getData(id);
				return setData(id, ObjectProperty::makeVarFromData(value, data.GetType()));
		}
		return false;
	}

	bool VLCollectionModel::setType(const std::string& propId, ObjectProperty::Type type)
	{
		if (auto& model = getObjectStorage().get(propId))
			if (ObjectProperty::ConvertVLType(model->getData()) != type)
			{
				auto newDataPtr = ObjectProperty::MakeVarPtr(type);
				auto newModel = VarModelFactory::Instance().Create(*newDataPtr);
				return setModel(propId, newModel) != nullptr;
			}
		return false;
	}

	QVariant VLCollectionModel::role(const VLVarModelPtr& m, int index, int role) const
	{
		// Index already checked by the base functional
		switch (role) {
			case RoleName:
				// use internal name instead of model name
				// because the model name is the proto's object name
				return QVariant(getId(index)->c_str());
			default:
				return Base::role(m, index, role);
		}
	}

	QVariant VLCollectionModel::roleValueStr(const VLVarModelPtr& m, int index) const
	{
		// use internal name instead of model name
		// because the model name is the proto's object name
		if (*getId(index) == "proto")
			return QVariant(
				utils::format_str("{%s}"
				, m->getTypeId().c_str()).c_str()
			);
		else
			return Base::roleValueStr(m, index);
	}
	// ======= End of Base interface =======

	// Redirects to remove
	bool VLCollectionModel::removeAt(int index)
	{
		if (auto id = getId(index))
			return remove(*id);
		return false;
	}
	// End of working with indexes

	// Object-specific setters
	vl::Var& VLCollectionModel::setData(const std::string& propId)
	{
		return setData(propId, MakePtr(vl::NullVar()));
	}

	vl::Var& VLCollectionModel::setData(const std::string& propId, const vl::Var& value)
	{
		return setData(propId, MakePtr(value));
	}

	vl::Var& VLCollectionModel::setData(
			const std::string& propId
			, const vl::VarPtr& ptr
			, const VoidCb& onBeforeAdd
			, const VoidCb& onAfterAdd
			)
	{
		return getData().Set(propId, ptr);
	}

	// Begin of Helper data-model setters
	const VLVarModelPtr& VLCollectionModel::setDataAndLoadModel(const std::string &propId, const vl::VarPtr &value)
	{
		setData(propId, value);
		return model(propId);
	}

	ModelInsertRet VLCollectionModel::addDataAndLoadModel(const std::string &propId, const vl::VarPtr &value)
	{
		const auto& data = getData();
		auto id = getFreeId(propId);
		return { id, setDataAndLoadModel(id, value) };
	}
	// End of Helper data-model setters

	bool VLCollectionModel::renameElement(const std::string& propId, const std::string& newId)
	{
		if (!mVLSubscriptionProcessor)
			return false;
		bool result = true;
		mVLSubscriptionProcessor->setOnAfterUpdate([&, propId] (const NotifContext& context) {
			auto& o = context.getNotifData();
			if (o.Get("rename").AsString().Val() == propId)
				if (o.Has("after"))
				{
					// If there is no such id entry or a model in the storage
					// Then nothing to rename and we should return false
					if (!o.Has("renamedInStorage"))
						result = false;
					return true;
				}
			return false;
		});
		return result && getData().RenameProperty(std::string(propId), newId);
	}

	bool VLCollectionModel::hasDataOwn(const std::string &propId) const
	{
		return getData().HasOwn(propId);
	}

	// ====== Begin of QAbstractListModel interface ======
	int VLCollectionModel::rowCount(const QModelIndex &parent) const
	{
		// For list models only the root node (an invalid parent) should return the list's size. For all
		// other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
		if (parent.isValid())
			return 0;
		auto sz = dataSize();
		LOCAL_VERBOSE("rowCount: " << sz);
		return sz;
	}

	QHash<int, QByteArray> VLCollectionModel::roleNames() const
	{
		QHash<int, QByteArray> roles = Base::roleNames();
		roles[RoleName] = "name";
		return roles;
	}
	// ====== End of QAbstractListModel interface ======

	bool VLCollectionModel::isCollection() const
	{
		return true;
	}

	VLCollectionModel *VLCollectionModel::asCollection()
	{
		return this;
	}

	// Begin of Data getters
	const vl::Object &VLCollectionModel::getData() const
	{
		if (const auto* parentModel = getParentModel().get())
			return parentModel->getData();
		return vl::nullObject;
	}

	vl::Object &VLCollectionModel::getData()
	{
		return const_cast<vl::Object&>(const_cast<const VLCollectionModel*>(this)->getData());
	}

	const vl::Var &VLCollectionModel::getData(const std::string &propId) const
	{
		return getData().Get(propId);
	}

	vl::Var &VLCollectionModel::getData(const std::string &propId)
	{
		return const_cast<vl::Var&>(const_cast<const VLCollectionModel*>(this)->getData(propId));
	}
	// End of Data getters

	std::string VLCollectionModel::getFreeId(const std::string &desiredId) const
	{
		return getObjectStorage().getIdList().getFreeId(desiredId);
	}

	// Remove a model with it's data from the object and this collection
	bool VLCollectionModel::remove(const std::string& propId)
	{
		if (!mVLSubscriptionProcessor)
			return false;
		bool result = true;
		mVLSubscriptionProcessor->setOnAfterUpdate([&] (const NotifContext& context) {
			auto& o = context.getNotifData();
			auto& l = context.getLocalData();
			if (o.Get("remove").AsString().Val() == propId)
			{
				if (o.Has("after"))
				{
					// If there is already no such element in the storage
					// Then nothing to remove and we should return false
					if (!l.Has("indexFromStorage") || !l.Has("removedFromStorage"))
						result = false;
					return true;
				}
			}
			return false;
		});
		return result && getData().RemoveProperty(propId);
	}

	// Redirects to remove
	bool VLCollectionModel::removeElement(const VLVarModel *childPtr)
	{
		if (auto id = getElementId(childPtr))
			return remove(*id);
		return false;
	}

	const vl::Var &VLCollectionModel::setElementValue(const VLVarModel *e, const vl::VarPtr &value)
	{
		if (auto id = getElementId(e))
			return setData(*id, value);
		return vl::emptyVar;
	}

	bool VLCollectionModel::setElementType(const VLVarModel *e, ObjectProperty::Type newType)
	{
		if (auto id = getElementId(e))
			return setType(*id, newType);
		return false;
	}

	// Removes model childPtr and replace it with a new having newType


	// ======= Begin of public slots =======
	void VLCollectionModel::onNameChanged()
	{
		auto i = getElementIndex(QObject::sender());
		if (i < 0)
		{
			LOG_ERROR("VLCollectionModel::onNameChanged slot received a signal from an unregistered sender");
			return;
		}
		QModelIndex index = this->index(i, 0, QModelIndex());
		emit dataChanged(index, index, QVector<int>() << RoleName);
	}

	void VLCollectionModel::onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last)
	{
		beginInsertRows(parent, first, last);
	}

	void VLCollectionModel::onRowsInserted(const QModelIndex &parent, int first, int last)
	{
		endInsertRows();
	}

	void VLCollectionModel::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
	{
		beginRemoveRows(parent, first, last);
	}

	void VLCollectionModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
	{
		endRemoveRows();
	}
	// ======= End of public slots =======

	// Store the model ptr in the storage
	// and init it with the parent (this collection's owner)
	// indexBefore is not used on the higher levels now
	const VLVarModelPtr &VLCollectionModel::putModel(const std::string &propId, const VLVarModelPtr &ptr, int indexBefore) {
		if (auto& ret = putToStorage(propId, ptr, indexBefore))
		{
			if (auto collectionOwner = getParentModel())
			{
				if (!ret->getParentModel())
					ret->Init(collectionOwner);
			}
			else
				LOCAL_ERROR("putModel(" << propId.c_str() << ") no parent present in the collection model. The putting model will not be initialized");
			return ret;
		}
		else
		{
			LOCAL_ERROR("putModel(" << propId.c_str() << ") return nullptr through the storage");
		}
		return nullVarModelPtr;
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

	bool VLCollectionModel::loadElementModels()
	{
		UpdateIdList();
		return getObjectStorage().getIdList().ForeachKey([&](auto& k) {
			if (!getObjectStorage().get(k))
				if (!loadElementModel(k))
					return false;
			return true;
		});
	}

	// Create a model and store it
	// indexBefore is not used on the higher levels now
	const VLVarModelPtr& VLCollectionModel::loadElementModel(const std::string& propId, int indexBefore)
	{
		auto& data = getData();
		if (!data.Has(propId))
			return nullVarModelPtr;
		auto& propData = data.Get(propId);
		bool complete = false;
		if (auto collectionOwner = getParentModel())
		{
			LOCAL_VERBOSE("Create model for '" << propId.c_str() << "'");
			if (propId == "proto")
				if (auto& m = loadProtoModel(propData, collectionOwner, indexBefore))
					return m;
			if (!complete)
			{
				// Container should be prepared to fit in the range
				auto& ptr = putModel(propId
							 , VarModelFactory::Instance().CreateEmpty(propData)
							 , indexBefore);
				return ptr;
			}
		}
		else
		{
			// Parent should exist
			assert(false);
		}
		return nullVarModelPtr;
	}

	// Called from loadElementModel for it's code reuse
	// indexBefore is not used on the higher levels now
	const VLVarModelPtr& VLCollectionModel::loadProtoModel(const vl::Var& data, const VLObjectVarModelPtr& collectionOwner, int indexBefore)
	{
		auto& protoData = data.AsObject();
		if (auto dmbModel = collectionOwner->getDataModel())
		{
			auto protoId = dmbModel->getDataModel().GetTypeId(protoData);
			if (auto& p = dmbModel->modelByTypeId(protoId))
				return putModel("proto", p, indexBefore);
			else
				LOCAL_WARNING(utils::format_str("Can't find or create a model for proto '%s'", protoId.c_str()).c_str());
		}
		else
		{
			// Any element in the hierarchy should have a link to the model
			LOCAL_ERROR("No owner of a model present during loading the proto model");
			assert(false);
		}
		return nullVarModelPtr;
	}

	std::unique_ptr<ModelStorageSubscriptionProcessor> VLCollectionModel::createStorageSubscriptionProcessor()
	{
		return std::make_unique<ObjectModelStorageSubscriptionProcessor>(*this);
	}

	const VLVarModelPtr &VLCollectionModel::setModel(const std::string &propId, const VLVarModelPtr &m)
	{
		if (!mVLSubscriptionProcessor)
			return nullVarModelPtr;

		if (!m)
		{
			LOG_ERROR("Null model passed to setModel('" << propId.c_str() << ")");
			return nullVarModelPtr;
		}

		vl::VarPtr dataPtr = nullptr;
		VLVarModelPtr modelPtr = m;

		bool standalone = false;
		bool hasParent = m->getParentModel() != nullptr;
		// Check if it is a standalone model
		if (!hasParent)
			if (auto owner = m->getDataModel())
			{
//				if (modelOverwriteRestricted)
//				{
//					if (owner->hasStandaloneModel(m.get()))
//						dataPtr = vl::MakePtr(m->getData());
//				}
//				else
					if (auto standaloneModel = owner->takeStandaloneModel(m.get()))
					{
						modelPtr = standaloneModel;
						dataPtr = vl::MakePtr(standaloneModel->getData());
						standalone = true;
					}
			}
		// Put a proto
		if (propId == "proto")
			dataPtr = vl::MakePtr(m->getData());

		if (!dataPtr)
			// Otherwise just take its data as a copy
			dataPtr = m->getData().Copy();
			// and then return a newly created model
			// We don't support sharing other models as prototypes

		// Send Qt list model insert rows signal
		// Set the data
		const VLVarModelPtr* result = &nullVarModelPtr;

		if (modelOverwriteRestricted && !standalone && hasParent)
		{
			// Put our modelPtr only if we have no such one
			result = &getModel(propId);
			modelPtr = nullptr;
		}
		if (modelPtr)
			mVLSubscriptionProcessor->setOnBeforeUpdate([&, propId] (const NotifContext& c) {
				auto& o = c.getNotifData();
				if (o.Get("add").AsString().Val() == propId
				|| o.Get("set").AsString().Val() == propId)
					if (o.Has("after"))
					{
						// Put the model if it is not restricted by a flag
						// or if there is no such model yet
						result = &putModel(propId, modelPtr);
						return true;
					}
				return false;
			});
		setData(propId, dataPtr);
		if (*result)
			return *result;
		else // If propId is already in the collection
			// then just load its model after setting the data
			return model(propId);
	}

	const VLVarModelPtr& VLCollectionModel::getModel(const std::string &propId) const
	{
		if (auto& m = getObjectStorage().get(propId))
			return m;
		if (propId == "proto")
			return nullVarModelPtr;
		if (auto proto = getModel("proto"))
			return proto->asObject()->getModel(propId);
		return nullVarModelPtr;
	}

	const VLVarModelPtr& VLCollectionModel::model(const std::string &propId)
	{
		if (hasDataOwn(propId))
		{
			if (auto& m = getObjectStorage().get(propId))
				return m;
			else
				return loadElementModel(propId);
		}
		if (propId == "proto")
			return nullVarModelPtr;
		if (auto proto = model("proto"))
			return proto->asObject()->model(propId);
		return nullVarModelPtr;
	}

	// ======= Begin of Qt interface =======
	bool VLCollectionModel::has(const QString& propId) const
	{
		return hasDataOwn(propId.toStdString());
	}

	QVariant VLCollectionModel::get(const QString& propId)
	{
		return QVariant::fromValue(model(propId.toStdString()).get());
	}

	const VLVarModelPtr& VLCollectionModel::set(const std::string &propId, VLVarModel *v)
	{
		if (!v)
		{
			LOG_ERROR("Null model passed to set('" << propId.c_str() << ")");
			return nullVarModelPtr;
		}
		return setModel(propId, v->shared_from_this());
	}

	QVariant VLCollectionModel::set(const QString &propId, const QVariant &data)
	{
		if (data.canConvert<VLVarModel*>())
			return QVariant::fromValue(set(propId.toStdString(), qvariant_cast<VLVarModel*>(data)).get());
		return QVariant::fromValue(setDataAndLoadModel(propId.toStdString(), ObjectProperty::makeVarFromData(data)).get());
	}

	// Adds a new model with propId and type to the end of the list
	// stores and return it
	// If the passed id is already taken
	// then take the free similar to the passed with an added suffix
	QVariant VLCollectionModel::add(const QString& propId, ObjectProperty::Type type)
	{
		auto propIdStd = propId.toStdString();
		if (auto ret = addDataAndLoadModel(propIdStd, ObjectProperty::MakeVarPtr(type)))
			return QVariant::fromValue(ret.data.get());
		return QVariant();
	}

	// Redirects to add(const QString&)
	QVariant VLCollectionModel::add(ObjectProperty::Type type)
	{
		return add("New prop", type);
	}

	// Redirects to remove(const std::string&)
	bool VLCollectionModel::remove(const QString &propId)
	{
		return remove(propId.toStdString());
	}
	// ======= End of Qt interface =======
}
