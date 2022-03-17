#ifndef LISTMODELSTORAGE_H
#define LISTMODELSTORAGE_H

#include <vector>
#include <unordered_map>
#include <memory>
#include "ModelStorage.h"

namespace dmb
{
	class ListModelStorage : public ModelStorage
	{
	public:
		~ListModelStorage();
		// Begin of ModelStorage interface
		int getIndex(const void* e) const override;
		int size() const override {
			return mElements.size();
		}
		inline bool empty() const override {
			return mElements.empty();
		}
		inline void resize(int newSize) {
			mElements.resize(newSize);
		}
		void clear() override;
		const VLVarModelPtr& operator[](int index) const override;
		VLVarModelPtr& at(int index) override;
		bool remove(int index) override;
		// End of ModelStorage interface

		const VLVarModelPtr& put(int index, const VLVarModelPtr& ptr, int indexBefore = -1);

	protected:
		std::vector<VLVarModelPtr> mElements;
		std::unordered_map<const void*, int> mElementIndex;
	};
	typedef std::shared_ptr<ListModelStorage> ListModelStoragePtr;
}
#endif // LISTMODELSTORAGE_H
