#ifndef COLLECTIONMODELSUBSCRIPTIONPROCESSOR_H
#define COLLECTIONMODELSUBSCRIPTIONPROCESSOR_H

// Default processor of VL notifications
// with logic for collections with own properties by default

#include "Observer.h"
#include "ModelsFwd.h"
#include "vl_fwd.h"
#include "NotifContext.h"

namespace dmb
{
	class CollectionModelSubscriptionProcessor : public vl::Observer
	{
	public:
		CollectionModelSubscriptionProcessor(VLCollectionModel& owner)
			: mOwner(&owner) {}

		~CollectionModelSubscriptionProcessor() = default;

		// Observer interface
		void Update(vl::Observable* sender, vl::VarPtr info = nullptr);

		inline void setOnBeforeUpdate(const BoolCbContext& cb) {
			mOnBeforeUpdate = cb;
		}

		inline void setOnAfterUpdate(const BoolCbContext& cb) {
			mOnAfterUpdate = cb;
		}

	protected:
		bool updateContext(vl::Var& info, const std::string& path = "");
		bool processUpdate(vl::Var& info, const std::string& path = "");

		inline VLCollectionModel& getOwner() {
			return *mOwner;
		}

		// Add
		virtual void onAdd();
		virtual void onBeforeAdd(const std::string& id);
		virtual void onAfterAdd(const std::string& id);
		virtual bool applyAdd(const std::string& id);
		virtual void onApplyAdd(const std::string &id);
		// Set
		virtual void onSet();
		virtual void onBeforeSet(const std::string& id);
		virtual void onAfterSet(const std::string& id);
		virtual bool applySet(const std::string& id);
		virtual void onApplySet(const std::string& id);

		// Remove
		virtual void onRemove();
		virtual void onBeforeRemove(const std::string& id);
		virtual void onAfterRemove(const std::string& id);
		virtual bool applyRemove(const std::string& id);
		virtual void onApplyRemove(const std::string& id);
		virtual bool doRemove( const std::string& id);

		// Rename
		// Rename notif processor
		virtual void onRename();
		virtual void onBeforeRename(const std::string& id, const std::string& newId);
		virtual void onAfterRename(const std::string& id, const std::string& newId);
		virtual bool applyRename(const std::string& id, const std::string& newId);
		virtual bool doRename(const std::string& id, const std::string& newId);

		// Clear
		virtual void onClear();
		virtual void onBeforeClear();
		virtual void onAfterClear();


		// Common functions for code reusage
		void doUpdateModel(const std::string& id);
		void onNoRelativePath(const std::string& id);
		void storeOldType(const std::string& id);

	protected:
		void processCommand();
		NotifContext& context();

	protected:
		// Data
		VLCollectionModel* mOwner;
		BoolCbContext mOnBeforeUpdate;
		BoolCbContext mOnAfterUpdate;

		// Command stack

		std::queue<NotifContext> mStack;
	};
}

#endif // COLLECTIONMODELSUBSCRIPTIONPROCESSOR_H
