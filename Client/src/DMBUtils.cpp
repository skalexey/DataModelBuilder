#include <qvariant.h>
#include "DMBUtils.h"
#include "vl.h"
#include "VarModelFactory.h"
#include "VLVarModel.h"

namespace dmb
{
	Utils* Utils::mInstance = nullptr;
	std::function<Utils*()> Utils::mCreator = [] {
		return new Utils();
	};

	Utils& Utils::Instance()
	{
		if (!mInstance)
			mInstance = mCreator();
		return *mInstance;
	}

	Utils::Utils(QObject *parent)
		: QObject{parent}
	{}
}
