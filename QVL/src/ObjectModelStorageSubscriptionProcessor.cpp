#include "ObjectModelStorageSubscriptionProcessor.h"

#include "vl.h"
#include "VLCollectionModel.h"
#include "VLObjectVarModel.h"

#include <utils/Log.h>
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("ObjectModelStorageSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})

namespace dmb
{
	ObjectModelStorageSubscriptionProcessor::ObjectModelStorageSubscriptionProcessor(VLCollectionModel &owner)
		: Base(owner)
	{}

	dmb::VLCollectionModel &dmb::ObjectModelStorageSubscriptionProcessor::getOwner() {
		return *dynamic_cast<VLCollectionModel*>(mOwner);
	}

	void ObjectModelStorageSubscriptionProcessor::onAfterModelPut(vl::Object& o)
	{
		auto& u = o.Get("modelPut");
		if (u.IsString())
		{
			auto modelId = u.AsString().Val();
			if (auto& m = getOwner().getObjectStorage().get(modelId))
				getOwner().onModelPut(m);
		}
		else
		{
			LOCAL_ERROR("Storage update caught by a collection which does not support other storage index type as Number");
		}
	}

	void ObjectModelStorageSubscriptionProcessor::onBeforeRemove(vl::Object& o)
	{
		auto& u = o.Get("modelRemove");
		if (u.IsString())
		{
			auto modelId = u.AsString().Val();
			if (auto& m = getOwner().getObjectStorage().get(modelId))
				getOwner().onModelBeforeRemove(m);
		}
		else
		{
			LOCAL_ERROR("Storage update 'modelRemove' caught by a collection which does not support other storage index type as Number");
		}
	}
}
