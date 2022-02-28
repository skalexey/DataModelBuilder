#include <cstdlib>
#include "JSONConverter.h"
#include "DMBCore.h"
#include "Log.h"
#ifdef LOG_ON
	#include "Utils.h"
#endif
#define DMB_LOG_ON
#ifdef DMB_LOG_ON
#include <iostream>
#define DMB_LOG_INFO(msg) std::cout << "[DMB I] " << msg << "\n"
#else
#define DMB_LOG_INFO(msg)
#endif


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

void dmb::Registry::Clear()
{
	mData.Clear();
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
	return converter.Store(mData, mTypeResolver, filePath, { params.pretty, false, params.storeTypeId });
}

std::string dmb::Content::JSONStr(const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.JSONStr(mData, mTypeResolver, { params.pretty, false, params.storeTypeId });
}

vl::Object& dmb::Content::GetData()
{
	return mData;
}

const vl::Object& dmb::Content::GetData() const
{
	return mData;
}

void dmb::Content::Clear()
{
	mData.Clear();
}

void dmb::Content::Init(const vl::Object& data, const TypeResolver& typeResolver)
{
	mData = data;
	mTypeResolver = typeResolver;
}

dmb::Model::Model()
	: mVarNodeRegistry(&mData)
	, mTypeResolver([&](const vl::Object& o) {
			return GetTypeId(o);
		}, [&](const std::string& typeRef) {
			if (auto node = mVarNodeRegistry.GetNode(typeRef))
			{
				if (auto data = node->GetData())
					return data->AsObject();
				else
					LOG_ERROR(Utils::FormatStr("Found node with null data during type resolution of '%s'", typeRef.c_str()));
			}
			return vl::nullObject;
		}
	)
{
	Init();
}

vl::Object& dmb::Model::GetType(const std::string& typeName) 
{
	return mRegistry.GetType(typeName).AsObject();
}

bool dmb::Model::Load(const std::string& fileName)
{
	DMB_LOG_INFO(Utils::FormatStr("dmb::Model::Load(%s)", fileName.c_str()));
	mIsLoaded = false;
	vl::JSONConverter converter;
	mData.Attach(&mVarNodeRegistry.GetTree());
	if (!converter.Load(mData, fileName, mTypeResolver))
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
	return converter.Store(mData, mTypeResolver, fileName, params);
}

std::string dmb::Model::JSONStr(const vl::CnvParams& params)
{
	vl::JSONConverter converter;
	return converter.JSONStr(mData, mTypeResolver, params);
}

std::string dmb::Model::GetTypeId(const vl::Object& obj) const
{
	// TODO: avoid iteration
	auto& context = mData;
	std::string type;
	auto search = [&](const vl::Var& r) {
		if (r.IsNull())
			return false;
		return !r.AsObject().ForeachProp(
			[&](const std::string& propName, const vl::Var& propVal) {
				if (propVal.AsObject() == obj)
				{
					type = propName;
					return false;
				}
				return true;
			});
	};
	if (search(context.Get("types")))
		return type;
	else if (search(context.Get("private")))
		return type;
	else // Construct a path to the object recursively
		if (auto node = mVarNodeRegistry.GetNode(obj))
			return node->GetPath();
	return type;
}

const vl::Object& dmb::Model::GetData()
{
	return mData;
}

void dmb::Model::Clear()
{
	mRegistry.Clear();
	mContent.Clear();
	mPrivate.Clear();
}

std::string dmb::Model::DataStr(bool pretty) const
{
	vl::JSONConverter converter;
	return converter.JSONStr(
		mData, mTypeResolver,{ pretty, false });
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
		mContent.Init(mData.Get("content").AsObject(), mTypeResolver);
	else
		mContent.Init(mData.Set("content", vl::Object()).AsObject(), mTypeResolver);
}

