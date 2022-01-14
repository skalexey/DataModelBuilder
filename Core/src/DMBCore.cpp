#include "JSONConverter.h"
#include "DMBCore.h"

namespace
{
	vl::NullVar emptyVar;
}

vl::Object& dmb::Registry::CreateType(const std::string& typeName)
{
	return mData.Set(typeName, vl::Object()).AsObject();
}

vl::Var& dmb::Registry::RegisterType(const std::string& typeName, vl::Var& type)
{
	return mData.Set(typeName, type);
}

vl::Object& dmb::Registry::GetType(const std::string& typeName)
{
	return mData.Get(typeName).AsObject();
}

bool dmb::Registry::ForeachType(const std::function<bool(const std::string&, vl::Object&)>& pred)
{
	return mData.ForeachProp([&](const std::string& name, vl::Var& value) {
		if (!pred(name, value.AsObject()))
			return false;
		return true;
	});
}

bool dmb::Registry::RemoveType(const std::string& typeName)
{
	return mData.RemoveProperty(typeName);
}

bool dmb::Registry::RenameType(const std::string& typeName, const std::string& newName)
{
	return mData.RenameProperty(typeName, newName);
}

bool dmb::Registry::HasType(const std::string& typeName)
{
	return mData.Has(typeName);
}

void dmb::Registry::Init(const vl::Object& data)
{
	mData = data;
}

vl::Object& dmb::Content::Add(const char* entityName, vl::Object& proto)
{
	vl::Object o;
	o.SetPrototype(proto);
	return mData.Set(entityName, o).AsObject();
}

vl::Var& dmb::Content::Add(const char* entityName, vl::Var& value)
{
	if (value.IsObject())
		return mData.Set(entityName, value.AsObject().Copy());
	else if (value.IsList())
		return mData.Set(entityName, value.AsList().Copy());
	else
		return mData.Set(entityName, value);
}

bool dmb::Content::Store(const char* filePath, const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.Store(mData, mContext, filePath, { false, params.storeTypeId, params.pretty });
}

std::string dmb::Content::JSONStr(const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.JSONStr(mData, mContext, { params.pretty, false, params.storeTypeId });
}

void dmb::Content::Init(const vl::Object& data, const vl::Object& context)
{
	mData = data;
	mContext = context;
}

vl::Object& dmb::Model::GetType(const char* typeName)
{
	return mRegistry.GetType(typeName).AsObject();
}

bool dmb::Model::Load(const char* fileName)
{
	vl::JSONConverter converter;
	if (!converter.Load(mData, fileName))
		return false;
	Init();
	return true;
}

bool dmb::Model::Store(const char* fileName, const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.Store(mData, mData, fileName, params);
}

std::string dmb::Model::JSONStr(const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.JSONStr(mData, mData, params);
}

void dmb::Model::Init()
{
	if (mData.Has("types"))
		mRegistry.Init(mData.Get("types").AsObject());
	else
		mRegistry.Init(mData.Set("types", vl::Object()).AsObject());

	if (mData.Has("private"))
		mPrivate.Init(mData.Get("private").AsObject());
	else
		mPrivate.Init(mData.Set("private", vl::Object()).AsObject());

	if (mData.Has("content"))
		mContent.Init(mData.Get("content").AsObject(), mData);
	else
		mContent.Init(mData.Set("content", vl::Object()).AsObject(), mData);
}
