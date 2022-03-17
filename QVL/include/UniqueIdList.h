#ifndef IDLIST_H
#define IDLIST_H

#include <cstddef>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>

namespace dmb
{
	class UniqueIdList
	{
	public:
		// Help types
		typedef std::unordered_map<std::string, int> KeyIndexContainer;
		struct FindIdRet {
			int index;
			KeyIndexContainer::const_iterator it;
			operator bool() { return index >= 0; }
		};
	public:
		inline size_t size() const {
			return mIdList.size();
		}
		inline bool empty() const {
			return mIdList.empty();
		}
		// Redirects to find(std::string)
		inline bool has(const std::string& id) const {
			return find(id);
		}
		// Returns id. Checks for size
		const std::string* at(int index) const;
		inline int index(const std::string& id) const {
			return find(id).index;
		}
		// Retirns id. Doesn't check for size
		const std::string& operator[](int index) const;

		int put(const std::string& id, int indexBefore = -1);
		inline bool remove(const std::string& id) {
			return erase(find(id));
		}

		bool rename(const std::string& id, const std::string& newId);
		void clear();
		typedef std::function<bool(const std::string&)> KeyPred;
		bool ForeachKey(const KeyPred& pred);
		bool ForeachKey(const KeyPred& pred) const;
		// Performance-aimed interface working with iterators
		FindIdRet find(const std::string& id) const;
		bool erase(const FindIdRet& ret);
		bool rename(KeyIndexContainer::const_iterator it, const std::string& newId);
		// End of Performance-aimed interface
		void swap(UniqueIdList& right);
		std::string getFreeId(const std::string& desiredId) const;

	protected:
	private:

		KeyIndexContainer mKeyIndex;
		std::vector<std::string> mIdList;
	};
}

#endif // IDLIST_H
