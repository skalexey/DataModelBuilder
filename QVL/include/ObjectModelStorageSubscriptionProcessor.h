#ifndef OBJECTMODELSTORAGESUBSCRIPTIONPROCESSOR_H
#define OBJECTMODELSTORAGESUBSCRIPTIONPROCESSOR_H

#include "ModelsFwd.h"
#include "ModelStorageSubscriptionProcessor.h"

namespace dmb
{
	// StringId-specific update processor
	class ObjectModelStorageSubscriptionProcessor : public ModelStorageSubscriptionProcessor
	{
		typedef ModelStorageSubscriptionProcessor Base;

	public:
		ObjectModelStorageSubscriptionProcessor(VLCollectionModel& owner);

	protected:
		void onAfterModelPut(vl::Object& o) override;
		void onBeforeRemove(vl::Object& o) override;

		VLCollectionModel& getOwner();
	};
}

#endif // OBJECTMODELSTORAGESUBSCRIPTIONPROCESSOR_H
