#include <assert.h>
#include <limits>
#include "ObjectModelStorage.h"
#include "VLVarModel.h"
#include "DMBModel.h"
#include "VLNotifications.h"
#include <utils/Log.h>
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("ObjectModelStorage")
LOG_STREAM([]{return qDebug();})
SET_LOCAL_LOG_LEVEL(debug)

namespace
{
	dmb::VLVarModelPtr nullVarModelPtr;
}

namespace dmb
{
	ObjectModelStorage::~ObjectModelStorage()
	{
		// Set the list empty to not allow remove any entry
		// externally during the remove process
		decltype(mElements) elements;
		decltype(mElementKey) elementKey;
		decltype(mIdList) idList;
		mElements.swap(elements);
		mElementKey.swap(elementKey);
		mIdList.swap(idList);

		auto root = getRoot();
		if (!root)
			LOCAL_DEBUG("Root is not available! Reset the parent of all children to be removed");
		auto parent = root ? root.get() : nullptr;
		for (auto& [id, e] : elements)
		{
			LOCAL_DEBUG(e.get() << "->setParent(" << parent << ")");
			e->setParent(parent);
		}
	}

	int ObjectModelStorage::getIndex(const void *e) const
	{
		auto it = mElementKey.find(e);
		if (it != mElementKey.end())
		{
			auto& key = it->second;
			auto ret = mIdList.find(key);
			assert(ret);
			return ret.index;
		}
		return -1;
	}

	const std::string* ObjectModelStorage::getId(const void *e) const
	{
		auto it = mElementKey.find(e);
		if (it != mElementKey.end())
			return &it->second;
		return nullptr;
	}

	const std::string* ObjectModelStorage::getId(int index) const
	{
		if (index >= 0 && index < mIdList.size())
			return &mIdList[index];
		return nullptr;
	}

	bool ObjectModelStorage::has(const std::string &propId) const
	{
		return mElements.find(propId) != mElements.end();
	}

	int ObjectModelStorage::listSize() const
	{
		return mIdList.size();
	}

	void ObjectModelStorage::clear()
	{
		if (empty())
			return;

		NOTIFY_BEFORE("clear", true, this)

		mElements.clear();
		mElementKey.clear();
		mIdList.clear();

		NOTIFY_AFTER(this)
	}

	// Store a model by ptr with id propId into the storage
	const VLVarModelPtr& ObjectModelStorage::put(const std::string& propId, const VLVarModelPtr& model, int indexBefore)
	{
		if (!model)
			LOCAL_WARNING("Warning! An attemption to put nullptr in the container chilren models storage");
		LOCAL_DEBUG("Put '" << propId.c_str() << "' (" << model.get() << ")");

		NOTIFY_BEFORE("modelPut", propId, this)

		if (!mIdList.has(propId))
			mIdList.put(propId, indexBefore);
		auto& result = mElements[propId] = model;
		mElementKey[model.get()] = propId;

		NOTIFY_AFTER(this)

		return result;
	}

	bool ObjectModelStorage::Foreach(const std::function<bool (const std::string &, const VLVarModelPtr &)> &pred) const
	{
		for (auto& [k, e] : mElements)
			if (!pred(k, e))
				return false;
		return true;
	}

	bool ObjectModelStorage::Foreach(const std::function<bool (const std::string &, const VLVarModelPtr &)> &pred)
	{
		for (auto& [k, e] : mElements)
			if (!pred(k, e))
				return false;
		return true;
	}

	bool ObjectModelStorage::rename(const std::string &propId, const std::string &newId)
	{
		if (mIdList.find(newId))
		{
			LOCAL_ERROR("Attemption to rename a property '" << propId.c_str() << "' with a new id which is already taken ('" << newId.c_str() << "'");
			return false;
		}
		auto ret = mIdList.find(propId);
		if (!ret)
			return false;

		INIT_NOTIFY_BEFORE
		NOTIFY_ADD("newId", newId)
		SEND_NOTIFY_BEFORE("rename", propId, this)

		auto it2 = mElements.find(propId);
		if (it2 != mElements.end())
		{
			auto ptr = it2->second;
			auto it3 = mElementKey.find(ptr.get());
			assert(it3 != mElementKey.end());
			it3->second = newId;
			mElements.erase(propId);
			mElements[newId] = ptr;
		}
		// The propId is actualy the reference to what we overwrite
		// so change it the last
		mIdList.rename(ret.it, newId);

		NOTIFY_AFTER(this)

		return true;
	}

	const VLVarModelPtr &ObjectModelStorage::operator[](int index) const
	{
		if (index < 0 || index >= mIdList.size())
			return nullVarModelPtr;
		auto it = mElements.find(mIdList[index]);
		assert(it != mElements.end());
		return it->second;
	}

	VLVarModelPtr &ObjectModelStorage::at(int index)
	{
		auto it = mElements.find(mIdList[index]);
		assert(it != mElements.end());
		return it->second;
	}

	const VLVarModelPtr &ObjectModelStorage::get(const std::string &propId) const
	{
		auto it = mElements.find(propId);
		if (it != mElements.end())
			return it->second;
		return nullVarModelPtr;
	}

	bool ObjectModelStorage::remove(int index)
	{
		return remove(mIdList[index]);
	}

	bool ObjectModelStorage::remove(const std::string &propId)
	{
		auto ret = mIdList.find(propId);
		if (!ret)
			return false;

		NOTIFY_BEFORE("modelRemove", propId, this);

		auto it2 = mElements.find(propId);
		if (it2 == mElements.end())
			return false;
		auto e = it2->second.get();
		mElementKey.erase(e);

		// Remove element from its Qt parent
		auto root = getRoot();
		auto parent = root ? root.get() : nullptr;
		LOCAL_DEBUG(e << "->setParent(" << parent << ")");
			e->setParent(parent);

		// Destroy the element
		mElements.erase(it2);

		// Clear the data further
		mIdList.erase(ret);

		// Send "after" notification
		NOTIFY_AFTER(this)

		return true;
	}
}
