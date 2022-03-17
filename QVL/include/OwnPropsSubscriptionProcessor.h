#ifndef OWNPROPSSUBSCRIPTIONPROCESSOR_H
#define OWNPROPSSUBSCRIPTIONPROCESSOR_H

#include "ModelsFwd.h"
#include "vl_fwd.h"
#include "CollectionModelSubscriptionProcessor.h"
#include "OwnPropsCollectionModel.h"

namespace dmb
{
	class OwnPropsSubscriptionProcessor : public CollectionModelSubscriptionProcessor
	{
		typedef CollectionModelSubscriptionProcessor Base;
	public:
		OwnPropsSubscriptionProcessor(OwnPropsCollectionModel& owner)
			: Base(owner) {}

	protected:
		// Base interface
		void onAfterAdd(const std::string& id) override;
		void onApplySet(const std::string& id) override;
		bool doRemove(const std::string& id) override;
		bool doRename(const std::string &id, const std::string &newId) override;
		void onAfterRename(const std::string& id, const std::string& newId) override;

	protected:
		inline OwnPropsCollectionModel& getOwner() {
			return *dynamic_cast<OwnPropsCollectionModel*>(mOwner);
		}
	};
}

#endif // OWNPROPSSUBSCRIPTIONPROCESSOR_H
