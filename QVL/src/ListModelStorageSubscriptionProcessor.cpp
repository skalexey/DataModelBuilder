#include "ListModelStorageSubscriptionProcessor.h"
#include "vl.h"
#include "VLCollectionModel.h"
#include "VLObjectVarModel.h"

#include "Log.h"
#ifdef LOG_ON
	#include <QDebug>
	#include "DMBModel.h"
#endif
LOG_TITLE("ListModelStorageSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})

namespace dmb
{
	void ListModelStorageSubscriptionProcessor::onAfterModelPut(vl::Object& o)
	{
		auto& u = o.Get("modelPut");
		if (u.IsNumber())
		{
			auto modelIndex = u.AsNumber().Val();
			if (auto& m = getOwner().getStorage().at(modelIndex))
				getOwner().onModelPut(m);
		}
		else
		{
			LOCAL_ERROR("Storage update catched by a collection which does not support other storage index type as Number");
		}
	}

	void ListModelStorageSubscriptionProcessor::onBeforeRemove(vl::Object& o)
	{
		auto& u = o.Get("modelRemove");
		if (u.IsNumber())
		{
			auto modelIndex = u.AsNumber().Val();
			if (auto& m = getOwner().getStorage().at(modelIndex))
				getOwner().onModelBeforeRemove(m);
		}
		else
		{
			LOCAL_ERROR("Storage update 'modelRemove' catched by a collection which does not support other storage index type as Number");
		}
	}
}
