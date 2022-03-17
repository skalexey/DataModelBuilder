#ifndef ALLPROPSSTORAGESUBSCRIPTIONPROCESSOR_H
#define ALLPROPSSTORAGESUBSCRIPTIONPROCESSOR_H

#include "ModelsFwd.h"
#include "ObjectModelStorageSubscriptionProcessor.h"

namespace dmb
{
	class AllPropsStorageSubscriptionProcessor : public ObjectModelStorageSubscriptionProcessor
	{
		typedef ObjectModelStorageSubscriptionProcessor Base;

	public:
		AllPropsStorageSubscriptionProcessor(AllPropsCollectionModel& owner);

	protected:
		// Base interface

		AllPropsCollectionModel& getOwner();
	};
}

#endif // ALLPROPSSTORAGESUBSCRIPTIONPROCESSOR_H
