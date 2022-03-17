#ifndef MODELSTORAGE_H
#define MODELSTORAGE_H

#include <memory>
#include "ModelsFwd.h"
#include "Observable.h"

namespace dmb
{
	class ModelStorage : public vl::Observable
	{
	public:
		virtual ~ModelStorage() {};
		virtual bool Init(const VLVarModelPtr& parent);
		virtual int getIndex(const void* e) const = 0;
		virtual int size() const = 0;
		virtual bool empty() const = 0;
		virtual void clear() = 0;
		virtual const VLVarModelPtr& operator[](int index) const = 0;
		virtual VLVarModelPtr& at(int index) = 0;
		virtual bool remove(int index) = 0;

	public:
		// Observable interface
		void Notify(vl::VarPtr info = nullptr) override;

	protected:
		inline VLVarModelPtr getParent() {
			return mParentModel.lock();
		}
		inline VLVarModelPtr getRoot() {
			return mRootModel.lock();
		}

	private:
		std::weak_ptr<VLVarModel> mParentModel;
		std::weak_ptr<VLVarModel> mRootModel;
	};
	typedef std::shared_ptr<ModelStorage> ModelStoragePtr;
}
#endif // MODELSTORAGE_H
