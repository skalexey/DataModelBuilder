#include "ModelStorageSubscriptionProcessor.h"
#include "vl.h"
#include "VLListModelInterface.h"
#include <utils/log.h>
#ifdef LOG_ON
	#include <QDebug>
	#include "DMBModel.h"
	#include "VLUtility.h"
#endif
LOG_TITLE("ModelStorageSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})
SET_LOCAL_LOG_LEVEL(error)

namespace dmb
{
	// Update from the storage
	// Index-specific
	void ModelStorageSubscriptionProcessor::Update(vl::Observable* sender, vl::VarPtr info)
	{
		if (auto collectionOwner = getOwner().getParentModel())
			LOCAL_DEBUG("Update storage notification on '" << collectionOwner->getStringId().c_str() << "'");
		LOCAL_DEBUG("msg: " << vl::VarToJSON(*info).c_str());

		if (auto& o = info->as<vl::Object>())
		{
			if (o.Get("who").as<vl::String>().Val() == "storage")
			{
				if (auto& u = o.Get("modelPut"))
				{
					if (o.Has("before"))
						onBeforeModelPut(o);
					else if (o.Has("after"))
						onAfterModelPut(o);
				}
				if (auto& u = o.Get("modelRemove"))
				{
					if (o.Has("before"))
						onBeforeRemove(o);
					if (o.Has("after"))
						onAfterRemove(o);
				}
				if (auto& u = o.Get("clear"))
				{
					if (o.Has("before"))
					{
						auto sz = getOwner().rowCount();
						if (sz >= 0)
						{
							getOwner().beginRemoveRows(QModelIndex(), 0, sz - 1);
							onBeforeClear(o);
							o.Set("size", int(sz));
						}
					}
					else if (o.Has("after"))
					{
						if (o.Has("size"))
						{
							getOwner().endRemoveRows();
							onAfterClear(o);
						}
					}
				}
				if (auto& u = o.Get("rename"))
				{
					if (o.Has("before"))
					{
						auto& id = o.Get("rename").as<vl::String>().Val();
						auto& newId = o.Get("newId").as<vl::String>().Val();
						onBeforeRename(id, newId);
					}
					else if (o.Has("after"))
					{
						auto& id = o.Get("rename").as<vl::String>().Val();
						auto& newId = o.Get("newId").as<vl::String>().Val();
						onAfterRename(id, newId);
					}
				}
			}
		}
	}

	void ModelStorageSubscriptionProcessor::onAfterModelPut(vl::Object &o)
	{
		// Nothing to do by default
	}

	void ModelStorageSubscriptionProcessor::onBeforeModelPut(vl::Object &o)
	{
		// Nothing to do by default
	}

	void ModelStorageSubscriptionProcessor::onAfterRemove(vl::Object &o)
	{
		// Nothing to do by default
	}

	void ModelStorageSubscriptionProcessor::onBeforeRemove(vl::Object &o)
	{
		// Nothing to do by default
	}

	void ModelStorageSubscriptionProcessor::onBeforeClear(vl::Object &o)
	{
		// Nothing to do by default
	}

	void ModelStorageSubscriptionProcessor::onAfterClear(vl::Object &o)
	{
		// Nothing to do by default
	}

	void ModelStorageSubscriptionProcessor::onBeforeRename(const std::string &id, const std::string &newId)
	{
		// Nothing to do by default
	}

	void ModelStorageSubscriptionProcessor::onAfterRename(const std::string &id, const std::string &newId)
	{
		// Nothing to do by default
	}
}
