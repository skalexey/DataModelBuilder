#ifndef ALLPROPSCOLLECTIONMODEL_H
#define ALLPROPSCOLLECTIONMODEL_H

#include "VLCollectionModel.h"

namespace dmb
{
	class AllPropsCollectionModel : public VLCollectionModel
	{
		typedef VLCollectionModel Base;

		friend class AllPropsSubscriptionProcessor;

		Q_OBJECT

	public:
		explicit AllPropsCollectionModel(QObject* parent, const ObjectModelStoragePtr& storage);
		explicit AllPropsCollectionModel(QObject* parent, const VLListModelInterface& storageOwner);

		// Begin of CollectionModel overrides
		void UpdateIdList() override;
		// Public model interface overrides of VLCollectionModel
		const VLVarModelPtr& getModel(const std::string& propId) const override;
		// Non-const version of getModel
		const VLVarModelPtr& model(const std::string& propId) override;
		// End of CollectionModel overrides
		// Begin of VLListModelInterface interface
		inline void clear() override {
			getObjectStorage().clear();
		}

	protected:
		// Subscription processors
		std::unique_ptr<CollectionModelSubscriptionProcessor> createVLSubacriptionProcessor() override;
		std::unique_ptr<ModelStorageSubscriptionProcessor> createStorageSubscriptionProcessor() override;

		int dataSize() const override;
		// TODO: adopt
//		vl::ObjectInsertRet addData(const std::string& propId, ObjectProperty::Type type);
	};
}

#endif // ALLPROPSCOLLECTIONMODEL_H
