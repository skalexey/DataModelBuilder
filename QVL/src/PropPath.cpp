#include "PropPath.h"

namespace dmb
{
	int PropPath::depth()
	{
		int level = -1;
		int cur = -1;
		size_t pos = 0;
		while (pos != std::string::npos)
		{
			level++;
			pos = mPath.find_first_not_of('.', cur + 1);
			cur = pos;
		}
		return level;
	}

	bool PropPath::isProtoPath()
	{
		int cur = -1;
		size_t pos = 0;
		while (pos != std::string::npos)
		{
			pos = mPath.find_first_of('.', ++cur);
			if (pos == std::string::npos)
				return mPath.substr(cur, mPath.size() - cur) == "proto";
			else
				if (mPath.substr(cur, pos - cur) != "proto")
					return false;
			cur = pos;
		}
		return true;
	}
}
