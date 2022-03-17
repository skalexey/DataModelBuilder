#include <assert.h>
#include "UniqueIdList.h"
#include "Utils.h"

namespace dmb
{


	const std::string* UniqueIdList::at(int index) const
	{
		if (index < 0 || index >= mIdList.size())
			return nullptr;
		return &mIdList[index];
	}

	const std::string &UniqueIdList::operator[](int index) const
	{
		return mIdList[index];
	}

	// Performance-aimed interface working with iterators
	UniqueIdList::FindIdRet UniqueIdList::find(const std::string &id) const
	{
		auto it = mKeyIndex.find(id);
		if (it == mKeyIndex.end())
			return { -1, it };
		return { it->second, it };
	}

	bool UniqueIdList::erase(const FindIdRet &ret)
	{
		if (ret.it == mKeyIndex.end())
			return false;
		if (ret.index < 0 && ret.index >= mIdList.size())
			return false;

		mKeyIndex.erase(ret.it);
		mIdList.erase(mIdList.begin() + ret.index);
		for (auto& [_, i] : mKeyIndex)
			if (i > ret.index)
				i--;

		return true;
	}

	bool UniqueIdList::rename(UniqueIdList::KeyIndexContainer::const_iterator it, const std::string& newId)
	{
		if (it == mKeyIndex.end())
			return false;
		int index = it->second;
		mKeyIndex[newId] = index;
		mIdList[index] = newId;
		mKeyIndex.erase(it);
		return true;
	}
	// End of Performance-aimed interface

	void UniqueIdList::swap(UniqueIdList &right)
	{
		mIdList.swap(right.mIdList);
		mKeyIndex.swap(right.mKeyIndex);
	}

	int UniqueIdList::put(const std::string &id, int indexBefore)
	{
		auto sz = size();
		int targetIndex = indexBefore;
		// If the wrong index passed then just put back
		if (targetIndex < 0 || targetIndex >= sz)
			targetIndex = sz;

		// If such id already exists in the list
		// Then remove it first from the previous position
		int indexRemoved = -1;
		auto it = mKeyIndex.find(id);
		if (it != mKeyIndex.end())
		{
			indexRemoved = it->second;
			if (targetIndex > indexRemoved)
				targetIndex--;
			mIdList.erase(mIdList.begin() + indexRemoved);
			for (auto& [s, i] : mKeyIndex)
				if (i > indexRemoved && i < targetIndex)
					i--;
		}

		mIdList.insert(mIdList.begin() + targetIndex, id);
		mKeyIndex[id] = targetIndex;
		return targetIndex;
	}

	bool UniqueIdList::rename(const std::string &id, const std::string &newId)
	{
		return rename(mKeyIndex.find(id), newId);
	}

	void UniqueIdList::clear()
	{
		mIdList.clear();
		mKeyIndex.clear();
	}

	bool UniqueIdList::ForeachKey(const KeyPred &pred) const
	{
		for (auto& k : mIdList)
			if (!pred(k))
				return false;
		return true;
	}

	bool UniqueIdList::ForeachKey(const KeyPred &pred)
	{
		return const_cast<const UniqueIdList*>(this)->ForeachKey(pred);
	}

	std::string UniqueIdList::getFreeId(const std::string& desiredId) const
	{
		auto newId = desiredId.empty() ? "new_prop" : desiredId;
		bool success = true;
		if (has(newId))
		{
			std::string tpl = newId;
			for (int i = 1; i < std::numeric_limits<short>::max(); i++)
				if ((success = !has(newId = Utils::FormatStr("%s %d", tpl.c_str(), i))))
					break;
		}
		if (success)
			return newId;
		return "";
	}
}
