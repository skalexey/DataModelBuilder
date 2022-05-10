#include <unordered_set>
#include <functional>
#include "AllPropsCollectionModel.h"
#include "VLObjectVarModel.h"
#include "DMBModel.h"
#include "VarModelFactory.h"
#include "vl.h"
#include "utils/Utils.h"
#include "AllPropsSubscriptionProcessor.h"
#include "AllPropsStorageSubscriptionProcessor.h"
#include "utils/Log.h"
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("AllPropsCollectionModel")
LOG_STREAM([]{return qDebug();})

namespace {
	dmb::VLVarModelPtr emptyVarModelPtr;
}

namespace dmb
{
	AllPropsCollectionModel::AllPropsCollectionModel(QObject *parent, const ObjectModelStoragePtr& storage)
		: Base(parent, storage)
	{}

	AllPropsCollectionModel::AllPropsCollectionModel(QObject *parent, const VLListModelInterface &storageOwner)
		: Base(parent, storageOwner)
	{}

	void AllPropsCollectionModel::UpdateIdList()
	{
		resetList([&]() {
			getObjectStorage().idList().clear();
			getData().ForeachProp(
				[&](const std::string& propName, const vl::Var& prop) {
					getObjectStorage().idList().put(propName);
					return true;
				}, true
			);
		});
	}

	const VLVarModelPtr &AllPropsCollectionModel::getModel(const std::string &propId) const
	{
		if (auto& m = getObjectStorage().get(propId))
			return m;
		return nullVarModelPtr;
	}

	const VLVarModelPtr &AllPropsCollectionModel::model(const std::string &propId)
	{
		if (auto& m = getObjectStorage().get(propId))
			return m;
		return loadElementModel(propId);
	}

	std::unique_ptr<CollectionModelSubscriptionProcessor> AllPropsCollectionModel::createVLSubacriptionProcessor()
	{
		return std::make_unique<AllPropsSubscriptionProcessor>(*this);
	}

	std::unique_ptr<ModelStorageSubscriptionProcessor> AllPropsCollectionModel::createStorageSubscriptionProcessor()
	{
		return std::make_unique<AllPropsStorageSubscriptionProcessor>(*this);
	}

	int AllPropsCollectionModel::dataSize() const
	{
		if (auto parent = getParentModel())
			return parent->sizeAll();
		return 0;
	}

//	vl::Var& AllPropsCollectionModel::setData(const std::string& propId, const vl::VarPtr& ptr)
//	{
//		return Base::setData(propId, ptr);
//	}

// TODO: adopt
//	vl::ObjectInsertRet VLCollectionModel::addData(const std::string &name, ObjectProperty::Type type)
//	{
//		auto id = getFreeId(name);
//		return { id, setData(id, ObjectProperty::MakeVarPtr(type)) };
//	}
}
