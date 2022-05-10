#include <functional>
#include "OwnPropsCollectionModel.h"
#include "VLObjectVarModel.h"
#include "DMBModel.h"
#include "VarModelFactory.h"
#include "vl.h"
#include "utils/Utils.h"
#include "OwnPropsSubscriptionProcessor.h"
#include "OwnPropsStorageSubscriptionProcessor.h"
#include "utils/Log.h"
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("OwnPropsCollectionModel")
LOG_STREAM([]{return qDebug();})

namespace {
	dmb::VLVarModelPtr emptyVarModelPtr;
}

namespace dmb
{
	OwnPropsCollectionModel::OwnPropsCollectionModel(QObject *parent, const ObjectModelStoragePtr& storage)
		: Base(parent, storage)
	{}

	OwnPropsCollectionModel::OwnPropsCollectionModel(QObject *parent, const VLListModelInterface &storageOwner)
		: Base(parent, storageOwner)
	{}

	int OwnPropsCollectionModel::dataSize() const
	{
		return VLCollectionModel::dataSize();
	}

	// See Base class description
	// indexBefore is always belongs to this list indexation space
	const VLVarModelPtr &OwnPropsCollectionModel::putToStorage(const std::string &propId, const VLVarModelPtr &model, int indexBefore) {
		// First put in our id list
		if (!mIdList.has(propId))
			mIdList.put(propId, indexBefore);
		int indexForSharedStorage = indexBefore;
		// The indexation in the shared storage may differ from the list in this object
		// so we need to convert indexBefore to that of the storage
		if (indexBefore >= 0 && indexBefore < mIdList.size())
			indexForSharedStorage = localToStorageSpace(indexBefore);
		return Base::putToStorage(propId, model, indexForSharedStorage);
	}

	int OwnPropsCollectionModel::localToStorageSpace(int index)
	{
		// Get the id of an element index to which we are going to convert this one
		if (auto id = mIdList.at(index))
		// Get the index of that element inside the shared storage
			if (auto ret = getObjectStorage().getIdList().find(*id))
				return ret.index;
		// Push back if the index succeding the last element is passed
		if (index == mIdList.size())
			return getObjectStorage().getIdList().size();
		return -1;
	}

	void OwnPropsCollectionModel::UpdateIdList()
	{
		resetList([&]() {
			mIdList.clear();
			getData().ForeachProp(
						[&](const std::string& propName, const vl::Var& prop) {
				mIdList.put(propName);
				return true;
			});
		});
	}

	std::string OwnPropsCollectionModel::getFreeId(const std::string &desiredId) const
	{
		return mIdList.getFreeId(desiredId);
	}

	const VLVarModelPtr &OwnPropsCollectionModel::getModel(const std::string &propId) const
	{
		if (!mIdList.find(propId))
			return nullVarModelPtr;
		if (auto& m = getObjectStorage().get(propId))
			return m;
		return nullVarModelPtr;
	}

	const VLVarModelPtr &OwnPropsCollectionModel::model(const std::string &propId)
	{
		if (hasDataOwn(propId))
		{
			if (auto& m = getObjectStorage().get(propId))
				return m;
			else
				return loadElementModel(propId);
		}
		return nullVarModelPtr;
	}

	bool OwnPropsCollectionModel::loadElementModels()
	{
		// Go through all props, but load only own
		UpdateIdList();
		return mIdList.ForeachKey([&](auto& k) {
			if (!getObjectStorage().get(k))
				if (!loadElementModel(k))
					return false;
			return true;
		});
	}

	std::unique_ptr<CollectionModelSubscriptionProcessor> OwnPropsCollectionModel::createVLSubacriptionProcessor()
	{
		return std::make_unique<OwnPropsSubscriptionProcessor>(*this);
	}

	std::unique_ptr<ModelStorageSubscriptionProcessor> OwnPropsCollectionModel::createStorageSubscriptionProcessor()
	{
		return std::make_unique<OwnPropsStorageSubscriptionProcessor>(*this);
	}
}
