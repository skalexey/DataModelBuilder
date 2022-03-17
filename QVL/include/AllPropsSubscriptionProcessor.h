#ifndef ALLPROPSSUBSCRIPTIONPROCESSOR_H
#define ALLPROPSSUBSCRIPTIONPROCESSOR_H

#include "ModelsFwd.h"
#include "vl_fwd.h"
#include "CollectionModelSubscriptionProcessor.h"
#include "AllPropsCollectionModel.h"

namespace dmb
{
	class AllPropsSubscriptionProcessor : public CollectionModelSubscriptionProcessor
	{
		typedef CollectionModelSubscriptionProcessor Base;

	public:
		AllPropsSubscriptionProcessor(AllPropsCollectionModel& owner)
			: Base(owner) {}

		~AllPropsSubscriptionProcessor() {}

	protected:
		// Base interface
		void onBeforeAdd(const std::string& id) override;
		void onAfterAdd(const std::string& id) override;

		bool applySet(const std::string& id) override;

		void onBeforeRemove(const std::string& id) override;
		void onAfterRemove(const std::string& id) override;
		bool doRemove(const std::string& id) override;

		void onBeforeRename(const std::string& id, const std::string& newId) override;
		void onAfterRename(const std::string& id, const std::string& newId) override;

	protected:
		inline AllPropsCollectionModel& getOwner() {
			return *dynamic_cast<AllPropsCollectionModel*>(mOwner);
		}
	};
}
#endif // ALLPROPSSUBSCRIPTIONPROCESSOR_H
