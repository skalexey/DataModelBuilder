#include "GlobalContext.h"

namespace dmb
{
	GlobalContext* GlobalContext::mInstance = nullptr;
	std::function<GlobalContext*()> GlobalContext::mCreator = [] {
		return new GlobalContext();
	};

	dmb::Model &GlobalContext::GetCurrentDataModel()
	{
		// TODO: use pointer or a dummy empty object
		return *mCurrentDataModel;
	}

	void GlobalContext::SetCurrentDataModel(Model *model)
	{
		mCurrentDataModel = model;
	}

	DMBModel* GlobalContext::GetCurrentModel()
	{
		return mCurrentModel;
	}

	void GlobalContext::SetCurrentModel(DMBModel *model)
	{
		mCurrentModel = model;
	}

	GlobalContext& GlobalContext::Instance()
	{
		if (!mInstance)
			mInstance = mCreator();
		return *mInstance;
	}
}
