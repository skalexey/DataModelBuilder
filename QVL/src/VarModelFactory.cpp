#include "ObjectProperty.h"
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

	VLObjectVarModelPtr VarModelFactory::CreateObject()
	{
		return std::make_shared<VLObjectVarModel>(vl::Object());
	}

	VLListVarModelPtr VarModelFactory::CreateList()
	{
		return std::make_shared<VLListVarModel>(vl::List());
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

	VLVarModelPtr VarModelFactory::createFromData(const QVariant &data)
	{
		vl::VarPtr v = ObjectProperty::makeVarFromData(data);
		return VarModelFactory::Instance().Create(*v);
	}

	VarModelFactory& VarModelFactory::Instance()
	{
		if (!mInstance)
			mInstance = mCreator();
		return *mInstance;
	}
}
