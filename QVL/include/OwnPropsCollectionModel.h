#ifndef OWNPROPSCOLLECTIONMODEL_H
#define OWNPROPSCOLLECTIONMODEL_H

#include "vl.h"
#include "UniqueIdList.h"
#include "VLCollectionModel.h"

namespace dmb
{
// Represents a collection of an object's own properties
// without those inherited from the proto chain
// Uses shared storage of AllPropsCollectionModel
	class OwnPropsCollectionModel : public VLCollectionModel
	{
		typedef VLCollectionModel Base;

		friend class OwnPropsSubscriptionProcessor;
		friend class OwnPropsStorageSubscriptionProcessor;

		Q_OBJECT

	public:
		explicit OwnPropsCollectionModel(QObject *parent, const ObjectModelStoragePtr& storage);
		explicit OwnPropsCollectionModel(QObject* parent, const VLListModelInterface& storageOwner);

		// Begin of VLListModelInterface
		int getElementIndex(const void* e) const override {
			if (auto id = getObjectStorage().getId(e))
				return getIndex(*id);
			return -1;
		}

		void clear() override {
			mIdList.clear();
		}
		// End of VLListModelInterface

		// Begin of CollectionModel overrides
		void UpdateIdList() override;
		std::string getFreeId(const std::string& desiredId) const override;
		const VLVarModelPtr& getModel(const std::string& propId) const override;
		// Non-const version of getModel
		const VLVarModelPtr& model(const std::string& propId) override;

	protected:
		const std::string* getId(int index) const override {
			return mIdList.at(index);
		}
		int getIndex(const std::string& propId) const override {
			return mIdList.index(propId);
		}
		int dataSize() const override;
		// See Base class description
		// indexBefore is always in this list indexation space
		const VLVarModelPtr& putToStorage(
				const std::string& propId
				, const VLVarModelPtr& model
				, int indexBefore = -1) override;

		// End of CollectionModel overrides
		int localToStorageSpace(int index);

	protected:
		bool loadElementModels() override;

	protected:
		// Subscription processors
		std::unique_ptr<CollectionModelSubscriptionProcessor> createVLSubacriptionProcessor() override;
		std::unique_ptr<ModelStorageSubscriptionProcessor> createStorageSubscriptionProcessor() override;

	protected:
		UniqueIdList& getIdList() {
			return mIdList;
		}

	private:
		// Data
		UniqueIdList mIdList;
	};
}

#endif // OWNPROPSCOLLECTIONMODEL_H
