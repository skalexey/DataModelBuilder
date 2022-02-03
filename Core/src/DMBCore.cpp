#include "JSONConverter.h"
#include "DMBCore.h"

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
	// TODO: optimize performance
	auto pred2 = [&](const std::string& s, const vl::Object& o) {
		return pred(s, const_cast<vl::Object&>(o));
	};
	return const_cast<const dmb::Registry*>(this)->ForeachType(pred2);
}

bool dmb::Registry::ForeachType(const std::function<bool(const std::string&, const vl::Object&)>& pred) const
{
	return mData.ForeachProp([&](const std::string& name, const vl::Var& value) {
		return pred(name, value.AsObject());
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

vl::Object& dmb::Registry::GetData()
{
	return mData;
}

const vl::Object& dmb::Registry::GetData() const
{
	return mData;
}

void dmb::Registry::Init(const vl::Object& data)
{
	mData = data;
}

vl::Object& dmb::Content::Add(const std::string& entityName, vl::Object& proto)
{
	if (Has(entityName))
		return vl::nullObject;
	vl::Object o;
	o.SetPrototype(proto);
	return mData.Set(entityName, o).AsObject();
}

vl::Var& dmb::Content::Add(const std::string& entityName, vl::Var& value)
{
	if (Has(entityName))
		return vl::emptyVar;
	if (value.IsObject())
		return mData.Set(entityName, value.AsObject().Copy());
	else if (value.IsList())
		return mData.Set(entityName, value.AsList().Copy());
	else
		return mData.Set(entityName, value);
}

int dmb::Content::ItemCount() const
{
	return mData.PropCount();
}

const vl::Var& dmb::Content::Get(const std::string& entityName) const
{
	return mData.Get(entityName);
}

bool dmb::Content::Has(const std::string& entityName) const
{
	return mData.Has(entityName);
}

bool dmb::Content::Remove(const std::string& entityName)
{
	return mData.RemoveProperty(entityName);
}

vl::Var& dmb::Content::Get(const std::string& entityName)
{
	return const_cast<vl::Var&>(const_cast<const dmb::Content*>(this)->Get(entityName));
}

bool dmb::Content::Rename(const std::string& entityName, const std::string& newName)
{
	return mData.RenameProperty(entityName, newName);
}

bool dmb::Content::ForeachItem(const std::function<bool(const std::string&, vl::Object&)>& pred)
{
	// TODO: optimize performance
	auto pred2 = [&](const std::string& s, const vl::Object& o) {
		return pred(s, const_cast<vl::Object&>(o));
	};
	return const_cast<const dmb::Content*>(this)->ForeachItem(pred2);
}

bool dmb::Content::ForeachItem(const std::function<bool(const std::string&, const vl::Object&)>& pred) const
{
	return mData.ForeachProp([&](const std::string& name, const vl::Var& value) {
		return pred(name, value.AsObject());
	});
}

bool dmb::Content::Store(const std::string& filePath, const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.Store(mData, mContext, filePath, { params.pretty, false, params.storeTypeId });
}

std::string dmb::Content::JSONStr(const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.JSONStr(mData, mContext, { params.pretty, false, params.storeTypeId });
}

vl::Object& dmb::Content::GetData()
{
	return mData;
}

const vl::Object& dmb::Content::GetData() const
{
	return mData;
}

void dmb::Content::Init(const vl::Object& data, const vl::Object& context)
{
	mData = data;
	mContext = context;
}

vl::Object& dmb::Model::GetType(const std::string& typeName)
{
	return mRegistry.GetType(typeName).AsObject();
}

bool dmb::Model::Load(const std::string& fileName)
{
	mIsLoaded = false;
	vl::JSONConverter converter;
	if (!converter.Load(mData, fileName))
		return false;
	Init();
	mIsLoaded = true;
	return true;
}

bool dmb::Model::IsLoaded() const
{
	return mIsLoaded;
}

bool dmb::Model::Store(const std::string& fileName, const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.Store(mData, mData, fileName, params);
}

std::string dmb::Model::JSONStr(const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.JSONStr(mData, mData, params);
}

std::string dmb::Model::GetTypeId(const vl::Object& obj) const
{
	return vl::GetTypeId(obj, mData);
}

const vl::Object& dmb::Model::GetData()
{
	return mData;
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
