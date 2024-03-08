#include "OwnPropsSubscriptionProcessor.h"
#include "vl.h"
#include "VLObjectVarModel.h"

#include <utils/log.h>
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("OwnPropsSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})

namespace dmb
{
	void OwnPropsSubscriptionProcessor::onAfterAdd(const std::string &id)
	{
		if (!getOwner().getIdList().has(id))
			getOwner().getIdList().put(id);
		Base::onAfterAdd(id);
	}

	void OwnPropsSubscriptionProcessor::onApplySet(const std::string &id)
	{
		// Do nothing as the shared model should be updated
		// from the AllPropsSubscriptionProcessor
	}

	bool OwnPropsSubscriptionProcessor::doRemove(const std::string& id)
	{
		// Rename only in our list as the storage is shared
		// and maintained by AllPropsCollection
		return getOwner().getIdList().remove(id);
	}

	bool OwnPropsSubscriptionProcessor::doRename(const std::string &id, const std::string &newId)
	{
		// Is called "before" the data is changed
		if (!getOwner().getIdList().rename(id, newId))
			return false;
		return true;
	}

	void OwnPropsSubscriptionProcessor::onAfterRename(const std::string& id, const std::string& newId)
	{
		// The model will be updated by AllPropsSubscriptionProcessor
		// The id in the
		// list should be already renamed in "before" case through doRename
	}


}
