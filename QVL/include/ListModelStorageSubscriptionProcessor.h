#ifndef LISTMODELSTORAGESUBSCRIPTIONPROCESSOR_H
#define LISTMODELSTORAGESUBSCRIPTIONPROCESSOR_H

#include "Observer.h"
#include "ModelsFwd.h"
#include "ModelStorageSubscriptionProcessor.h"

namespace dmb
{
	// Index-specific update processor
	class ListModelStorageSubscriptionProcessor : public ModelStorageSubscriptionProcessor
	{
		typedef ModelStorageSubscriptionProcessor Base;

	public:
		ListModelStorageSubscriptionProcessor(VLListModelInterface& owner)
			: Base(owner) {}

	protected:
		void onAfterModelPut(vl::Object& o) override;
		void onBeforeRemove(vl::Object& o) override;

		VLListModelInterface& getOwner() {
			return *mOwner;
		}
	};
}

#endif // LISTMODELSTORAGESUBSCRIPTIONPROCESSOR_H
