#pragma once

#include <functional>
#include <string>
#include "vl.h"
#include "JSONDefs.h"

namespace dmb
{
	class Model;

	class Registry
	{
		friend class Model;

	public:
		vl::Object& CreateType(const std::string&);
		vl::Var& RegisterType(const std::string&, vl::Var& type);
		vl::Object& GetType(const std::string&);
		bool ForeachType(const std::function<bool(const std::string&, vl::Object&)>& pred);
		bool ForeachType(const std::function<bool(const std::string&, const vl::Object&)>& pred) const;
		bool RemoveType(const std::string& typeName);
		bool RenameType(const std::string& typeName, const std::string& newName);
		bool HasType(const std::string& typeName);
		// TODO: think how to keep int under the hood
		vl::Object& GetData();
		const vl::Object& GetData() const;

	protected:
		void Init(const vl::Object& data);

	protected:
		vl::Object mData;
	};

	class Content
	{
		friend class Model;

	public:
		// Create an object of type proto and put it into the content
		vl::Object& Add(const std::string&, vl::Object& proto);
		// Put any value as it is. Objects and lists are copied
		vl::Var& Add(const std::string&, vl::Var& value);
		int ItemCount() const;
		vl::Var& Get(const std::string& entityName);
		const vl::Var& Get(const std::string& entityName) const;
		bool Has(const std::string& entityName) const;
		bool Remove(const std::string& entityName);
		bool Rename(const std::string& entityName, const std::string& newName);
		bool ForeachItem(const std::function<bool(const std::string&, vl::Object&)>& pred);
		bool ForeachItem(const std::function<bool(const std::string&, const vl::Object&)>& pred) const;
		bool Store(const std::string& filePath, const vl::CnvParams& params = vl::CnvParams());
		std::string JSONStr(const vl::CnvParams& params = vl::CnvParams());
		// TODO: TODO: think how to keep int under the hood
		vl::Object& GetData();
		const vl::Object& GetData() const;

	protected:
		void Init(const vl::Object& data, const vl::Object& context);

	protected:
		vl::Object mData;
		vl::Object mContext;
	};

	class Model
	{
	public:
		Model() { Init(); }
		Registry& GetRegistry() { return mRegistry; }
		Registry& GetPrivateScope() { return mPrivate; }
		Content& GetContent() { return mContent; }
		vl::Object& GetType(const std::string& typeName);
		bool Load(const std::string& fileName);
		bool IsLoaded() const;
		bool Store(const std::string& fileName, const vl::CnvParams& params = vl::CnvParams());
		std::string JSONStr(const vl::CnvParams& params = vl::CnvParams());
		std::string GetTypeId(const vl::Object& obj) const;
		const vl::Object& GetData();

	protected:
		void Init();

	protected:
		vl::Object mData;
		Registry mRegistry;
		Registry mPrivate;
		Content mContent;
		bool mIsLoaded = false;
	};
}