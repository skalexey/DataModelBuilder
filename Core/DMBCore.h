#pragma once

#include "vl.h"
#include "JSONDefs.h"

namespace dmb
{
	class Model;

	class Registry
	{
		friend class Model;

	public:
		vl::Object& CreateType(const char* typeName);
		vl::Var& RegisterType(const char* typeName, vl::Var& type);
		vl::Object& GetType(const char* typeName);

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
		vl::Object& Add(const char* entityName, vl::Object& proto);
		// Put any value as it is. Objects and lists are copied
		vl::Var& Add(const char* entityName, vl::Var& value);
		bool Store(const char* filePath, const vl::CnvParams& params = vl::CnvParams());
		std::string JSONStr(const vl::CnvParams& params = vl::CnvParams());

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
		vl::Object& GetType(const char* typeName);
		bool Load(const char* fileName);
		bool Store(const char* fileName, const vl::CnvParams& params = vl::CnvParams());
		std::string JSONStr(const vl::CnvParams& params = vl::CnvParams());

	protected:
		void Init();

	protected:
		vl::Object mData;
		Registry mRegistry;
		Registry mPrivate;
		Content mContent;
	};
}