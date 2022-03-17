#ifndef PROPPATH_H
#define PROPPATH_H

#include <string>

namespace dmb
{
	class PropPath
	{
	public:
		PropPath() = default;
		PropPath(const std::string& path) : mPath(path) {}

		int depth();
		bool isProtoPath();

	private:
		std::string mPath;
	};
}

#endif // PROPPATH_H
