#include "AllPropsStorageSubscriptionProcessor.h"
#include "vl.h"
#include "AllPropsCollectionModel.h"

#include "Log.h"
#ifdef LOG_ON
	#include <QDebug>
	#include "DMBModel.h"
#endif
LOG_TITLE("AllPropsStorageSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})

namespace dmb
{
	AllPropsStorageSubscriptionProcessor::AllPropsStorageSubscriptionProcessor(AllPropsCollectionModel &owner)
		: Base(owner)
	{}

	AllPropsCollectionModel &AllPropsStorageSubscriptionProcessor::getOwner() {
		return *dynamic_cast<AllPropsCollectionModel*>(mOwner);
	}
}
