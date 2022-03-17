#ifndef OBJECTMODELSTORAGE_H
#define OBJECTMODELSTORAGE_H

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "ModelStorage.h"
#include "UniqueIdList.h"

namespace dmb
{
	class ObjectModelStorage: public ModelStorage
	{
	public:
		~ObjectModelStorage();
		// Begin of ModelStorage int0erface
		int getIndex(const void* e) const override;
		inline int size() const override {
			return mElements.size();
		}
		inline bool empty() const override {
			return mElements.empty();
		}
		void clear() override;
		const VLVarModelPtr& operator[](int index) const override;
		VLVarModelPtr& at(int index) override;
		const VLVarModelPtr& get(const std::string& propId) const;
		bool remove(int index) override;
		// End of ModelStorage interface
		bool remove(const std::string& propId);
		inline const UniqueIdList& getIdList() const {
			return mIdList;
		}
		inline UniqueIdList& idList() {
			return mIdList;
		}
		const std::string* getId(const void* e) const;
		const std::string* getId(int index) const;
		bool has(const std::string& propId) const;

		// Store a model by ptr with id propId into the storage
		const VLVarModelPtr& put(
				const std::string& propId
				, const VLVarModelPtr& model
				, int indexBefore = -1
		);

		bool Foreach(const std::function<bool(const std::string&, const VLVarModelPtr&)>& pred);
		bool Foreach(const std::function<bool(const std::string&, const VLVarModelPtr&)>& pred) const;
		bool rename(const std::string& propId, const std::string& newId);
		int listSize() const;

	protected:
		std::unordered_map<std::string, VLVarModelPtr> mElements;
		std::unordered_map<const void*, std::string> mElementKey;
		UniqueIdList mIdList;
	};
	typedef std::shared_ptr<ObjectModelStorage> ObjectModelStoragePtr;
}
#endif // OBJECTMODELSTORAGE_H
