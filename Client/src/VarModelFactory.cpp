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

	VLVarModelPtr VarModelFactory::CreateEmpty(const vl::Var &v)
	{
		VLVarModelPtr ptr = nullptr;
		if (v.IsObject())
			ptr = std::dynamic_pointer_cast<VLVarModel>(std::make_shared<VLObjectVarModel>());
		else if (v.IsList())
			ptr = std::dynamic_pointer_cast<VLVarModel>(std::make_shared<VLListVarModel>());
		else
			ptr = std::make_shared<VLVarModel>();
		return ptr;
	}

	VLVarModelPtr VarModelFactory::Create(const vl::Var &v)
	{
		VLVarModelPtr ptr = nullptr;
		if (v.IsObject())
			ptr = std::dynamic_pointer_cast<VLVarModel>(std::make_shared<VLObjectVarModel>(v));
		else if (v.IsList())
			ptr = std::dynamic_pointer_cast<VLVarModel>(std::make_shared<VLListVarModel>(v));
		else
			ptr = std::make_shared<VLVarModel>(v);
		return ptr;
	}

//	VLVarModel* VarModelFactory::CreateFreeQtObject(const vl::Var &v)
//	{
//		VLVarModel* ptr = nullptr;
//		if (v.IsObject())
//			ptr = new VLObjectVarModel;
//		else if (v.IsList())
//			ptr = new VLListVarModel;
//		else
//			// TODO: make smth like VLVarModel<bool> or VLBoolModel
//			ptr = new VLVarModel;
//		return ptr;
//	}

	VarModelFactory& VarModelFactory::Instance()
	{
		if (!mInstance)
			mInstance = mCreator();
		return *mInstance;
	}
}
