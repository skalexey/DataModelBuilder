#ifndef OWNPROPSSTORAGESUBSCRIPTIONPROCESSOR_H
#define OWNPROPSSTORAGESUBSCRIPTIONPROCESSOR_H

#include "ModelsFwd.h"
#include "ObjectModelStorageSubscriptionProcessor.h"

namespace dmb
{
	class OwnPropsStorageSubscriptionProcessor : public ObjectModelStorageSubscriptionProcessor
	{
		typedef ObjectModelStorageSubscriptionProcessor Base;

	public:
		OwnPropsStorageSubscriptionProcessor(OwnPropsCollectionModel& owner);

	protected:
		// Base interface
		void onAfterClear(vl::Object& o) override;

		OwnPropsCollectionModel& getOwner();
	};
}

#endif // OWNPROPSSTORAGESUBSCRIPTIONPROCESSOR_H
