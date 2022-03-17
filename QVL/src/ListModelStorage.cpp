#include "ListModelStorage.h"
#include "VLVarModel.h"
#include "DMBModel.h"

#include "Log.h"
#ifdef LOG_ON
	#include <QDebug>
	#include "VLObjectVarModel.h"
#endif
LOG_STREAM([]{return qDebug();})
LOG_TITLE("ListModelStorage")
SET_LOG_DEBUG(true)

namespace dmb
{
	ListModelStorage::~ListModelStorage()
	{
		// Set the list empty to not allow remove any entry
		// externally during the remove process
		decltype(mElements) elements;
		decltype(mElementIndex) elementIndex;
		mElements.swap(elements);
		mElementIndex.swap(elementIndex);

		auto root = getRoot();
		if (!root)
			LOCAL_DEBUG("Root is not available! Reset the parent of all children to be removed");
		auto parent = root ? root.get() : nullptr;
		for (auto& e : elements)
		{
			LOCAL_DEBUG(e.get() << "->setParent(" << parent << ")");
			e->setParent(parent);
		}
	}

	int ListModelStorage::getIndex(const void *e) const
	{
		auto it = mElementIndex.find(e);
		if (it != mElementIndex.end())
			return it->second;
		return -1;
	}



	void ListModelStorage::clear()
	{
		// Notify all storage holders
		vl::Object info;
		auto ptr = vl::MakePtr(info);
		info.Set("clear", true);
		// Send "before" notification
		info.Set("before", true);
		Notify(ptr);

		mElements.clear();
		mElementIndex.clear();

		// Send "after" notification
		info.RemoveProperty("before");
		info.Set("after", true);
		Notify(ptr);
	}

	const VLVarModelPtr& ListModelStorage::put(int index, const VLVarModelPtr& model, int indexBefore)
	{
		if (!model)
			LOCAL_WARNING("An attemption to put nullptr in the container chilren models storage");

		// Notify all storage holders
		vl::Object info;
		auto ptr = vl::MakePtr(info);
		info.Set("modelPut", index);
		// Send "before" notification
		info.Set("before", true);
		Notify(ptr);

		auto sz = mElements.size();
		if (indexBefore >= 0)
		{
			for (auto& [p, i] : mElementIndex)
				if (i >= indexBefore)
					i++;
			mElements.resize(sz + 1);
			sz++;
			for (int i = sz - 2; i >= indexBefore && i >= 0; i--)
				mElements[i + 1] = mElements[i];
		}
		else if (index >= sz)
			mElements.resize(index + 1);
		auto& result = mElements[index] = model;
		mElementIndex[model.get()] = index;

		// Send "after" notification
		info.RemoveProperty("before");
		info.Set("after", true);
		Notify(ptr);

		return result;
	}

	const VLVarModelPtr &ListModelStorage::operator[](int index) const
	{
		return mElements[index];
	}

	VLVarModelPtr &ListModelStorage::at(int index)
	{
		return mElements[index];
	}

	bool ListModelStorage::remove(int index)
	{
		if (index < 0 || index >= mElements.size())
			return false;

		// Notify all storage holders
		vl::Object info;
		vl::VarPtr ptr = vl::MakePtr(info);
		info.Set("modelRemove", index);
		// Send "before" notification
		info.Set("before", true);
		Notify(ptr);

		auto e = mElements[index].get();
		mElementIndex.erase(e);

		// Remove element from its Qt parent
		auto root = getRoot();
		auto parent = root ? root.get() : nullptr;
		LOCAL_DEBUG(e << "->setParent(" << parent << ")");
			e->setParent(parent);

		// Destroy the element
		mElements.erase(mElements.begin() + index);
		for (auto& [_, i] : mElementIndex)
			if (i > index)
				i--;

		// Send "after" notification
		info.RemoveProperty("before");
		info.Set("after", true);
		Notify(ptr);

		return true;
	}
}
