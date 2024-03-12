
#include "ModelStorage.h"
#include "VLVarModel.h"
#include "DMBModel.h"
#include <utils/log.h>
#ifdef LOG_ON
	#include <QDebug>
#endif
LOG_TITLE("ModelStorage")
LOG_STREAM([]{return qDebug();})
SET_LOCAL_LOG_LEVEL(debug)

namespace dmb
{
	bool ModelStorage::Init(const VLVarModelPtr &parent)
	{
		LOCAL_DEBUG("Init " << parent.get());
		if (mParentModel.lock() == parent)
			return false;
		mParentModel = parent;
		if (parent)
		{
			if (auto owner = parent->getDataModel())
				mRootModel = owner->getRoot();
			else
				LOCAL_WARNING("Attempt to init a storage with null owner of a parent");

		}
		else
		{
			LOCAL_WARNING("Attempt to init a storage with null parent");
		}
		return true;
	}

	void ModelStorage::Notify(vl::VarPtr info)
	{
		info->as<vl::Object>().Set("who", "storage");
		Observable::Notify(info);
	}

}
