#include "OwnPropsStorageSubscriptionProcessor.h"
#include "vl.h"
#include "OwnPropsCollectionModel.h"

#include <utils/log.h>
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("OwnPropsStorageSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})

namespace dmb
{
	OwnPropsStorageSubscriptionProcessor::OwnPropsStorageSubscriptionProcessor(OwnPropsCollectionModel &owner)
		: Base(owner)
	{}

	OwnPropsCollectionModel &OwnPropsStorageSubscriptionProcessor::getOwner() {
		return *dynamic_cast<OwnPropsCollectionModel*>(mOwner);
	}

	void OwnPropsStorageSubscriptionProcessor::onAfterClear(vl::Object& o)
	{
		getOwner().getIdList().clear();
	}
}
