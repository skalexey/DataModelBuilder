#pragma once

// Default processor of VL notifications
// with logic for collections with own properties by default

#include "Observer.h"
#include "ModelsFwd.h"
#include "vl_fwd.h"
#include "NotifContext.h"
#include "CollectionModelSubscriptionProcessor.h"

namespace dmb
{
	class ListModelSubscriptionProcessor : public CollectionModelSubscriptionProcessor
	{
		using Base = CollectionModelSubscriptionProcessor;

	public:
		ListModelSubscriptionProcessor(VLListModelInterface& owner)
			: Base(owner) {}

		~ListModelSubscriptionProcessor() = default;

	protected:
		VLListModel& getOwner();

		// Add
		void onAdd() override;
		virtual void onBeforeAdd(int index);
		virtual void onAfterAdd(int index);
		virtual bool applyAdd(int index);
		virtual void onApplyAdd(int index);
		// Set
		void onSet() override;
		virtual void onBeforeSet(int index);
		virtual void onAfterSet(int index);
		virtual bool applySet(int index);
		virtual void onApplySet(int index);

		// Remove
		void onRemove() override;
		virtual void onBeforeRemove(int index);
		virtual void onAfterRemove(int index);
		virtual bool applyRemove(int index);
		virtual void onApplyRemove(int index);
		virtual bool doRemove( int index);

		// Common functions for code reusage
		void doUpdateModel(int index);
		void storeOldType(int index);

	protected:
		NotifContext& context();

	protected:
	};
}

