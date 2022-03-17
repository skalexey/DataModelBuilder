#ifndef MODELSFWD_H
#define MODELSFWD_H

#include <memory>
#include <functional>

namespace dmb {
	class VLVarModel;
	typedef std::shared_ptr<VLVarModel> VLVarModelPtr;

	class VLObjectVarModel;
	typedef std::shared_ptr<VLObjectVarModel> VLObjectVarModelPtr;

	class VLListVarModel;
	typedef std::shared_ptr<VLListVarModel> VLListVarModelPtr;

	class VLListModelInterface;
	class VLListModel;
	class VLCollectionModel;
	class OwnPropsCollectionModel;
	class AllPropsCollectionModel;

	class CollectionModelSubscriptionProcessor;
	class OwnPropsSubscriptionProcessor;
	class AllPropsSubscriptionProcessor;

	class ModelStorage;
	class ObjectModelStorage;
	class ListModelStorage;

	class ModelStorageSubscriptionProcessor;
	class ObjectModelStorageSubscriptionProcessor;
	class ListModelStorageSubscriptionProcessor;

	class DMBModel;

	class NotifContext;

	typedef std::function<void()> VoidCb;
	typedef std::function<void(int)> VoidCbInt;
	typedef std::function<bool(const NotifContext&)> BoolCbContext;
}

#endif // MODELSFWD_H
