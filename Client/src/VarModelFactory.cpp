#include "VarModelFactory.h"
#include "VLVarModel.h"
#include "VLObjectVarModel.h"
#include "VLListVarModel.h"

namespace dmb
{
	VarModelFactory* VarModelFactory::mInstance = nullptr;
	std::function<VarModelFactory*()> VarModelFactory::mCreator = [] {
		return new VarModelFactory();
	};

	VLVarModelPtr VarModelFactory::Create(const vl::Var &v)
	{
		VLVarModelPtr ptr = nullptr;
		if (v.IsObject())
			ptr = std::dynamic_pointer_cast<VLVarModel>(std::make_shared<VLObjectVarModel>());
		else if (v.IsList())
			ptr = std::dynamic_pointer_cast<VLVarModel>(std::make_shared<VLListVarModel>());
		else
			// TODO: make smth like VLVarModel<bool> or VLBoolModel
			ptr = std::make_shared<VLVarModel>();
		return ptr;
	}

	VarModelFactory& VarModelFactory::Instance()
	{
		if (!mInstance)
			mInstance = mCreator();
		return *mInstance;
	}
}
