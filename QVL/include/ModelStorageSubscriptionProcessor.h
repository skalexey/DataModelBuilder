#ifndef MODELSTORAGESUBSCRIPTIONPROCESSOR_H
#define MODELSTORAGESUBSCRIPTIONPROCESSOR_H

#include "Observer.h"
#include "ModelsFwd.h"

namespace dmb
{
	// Processes update from the storage
	// All it do now is call some common code for collections
	//	onModelPut
	//	onModelBeforeRemove

	class ModelStorageSubscriptionProcessor : public vl::Observer
	{
	public:
		ModelStorageSubscriptionProcessor(VLListModelInterface& owner)
			: mOwner(&owner) {}

		// Observer interface
		void Update(vl::Observable* sender, vl::VarPtr info = nullptr);

	protected:
		virtual void onAfterModelPut(vl::Object& o);
		virtual void onBeforeModelPut(vl::Object& o);
		virtual void onAfterRemove(vl::Object& o);
		virtual void onBeforeRemove(vl::Object& o);
		virtual void onBeforeClear(vl::Object& o);
		virtual void onAfterClear(vl::Object& o);
		virtual void onBeforeRename(const std::string& id, const std::string& newId);
		virtual void onAfterRename(const std::string& id, const std::string& newId);

		inline VLListModelInterface& getOwner() {
			return *mOwner;
		}

	protected:
		// Data
		VLListModelInterface* mOwner;
	};
}

#endif // MODELSTORAGESUBSCRIPTIONPROCESSOR_H
