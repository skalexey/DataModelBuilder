#pragma once

#include <memory>
#include <cstddef>
#include <functional>
#include <string>
#include "vl.h"
#include "JSONDefs.h"
#include "VLBackwardTraversable.h"
#include "TypeResolver.h"

namespace dmb
{
	class Model;

	// Util class for working with types as vl objects
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
		// TODO: think how to keep it under the hood
		vl::Object& GetData();
		const vl::Object& GetData() const;
		void Clear(bool recursive = false);

	protected:
		void Init(const vl::Object& data);

	protected:
		vl::Object mData;
	};

	class Model;

	// Util class for working with content as a set of vl variables
	// Content can be stored separately of its model
	class Content
	{
		friend class Model;

	public:
		// Create an object of type proto and put it into the content
		vl::Object& Add(const std::string&, vl::Object& proto);
		// Put any value as it is. Objects and lists are copied
		vl::Var& Add(const std::string&, vl::Var& value);
		std::size_t ItemCount() const;
		vl::Var& Get(const std::string& entityName);
		const vl::Var& Get(const std::string& entityName) const;
		bool Has(const std::string& entityName) const;
		bool Remove(const std::string& entityName);
		bool Rename(const std::string& entityName, const std::string& newName);
		bool ForeachItem(const std::function<bool(const std::string&, vl::Object&)>& pred);
		bool ForeachItem(const std::function<bool(const std::string&, const vl::Object&)>& pred) const;
		bool Store(const std::string& filePath, const vl::CnvParams& params = vl::CnvParams());
		std::string JSONStr(const vl::CnvParams& params = vl::CnvParams());
		// TODO: think how to keep it under the hood
		vl::Object& GetData();
		const vl::Object& GetData() const;
		void Clear(bool recursive = false);

	protected:
		void Init(const vl::Object& data, const TypeResolver& typeResolver);

	protected:
		vl::Object mData;
		TypeResolver mTypeResolver;
	};

	// Util class for working with data model as a vl object
	// with types and content fields
	// You can store or load a model from JSON
	class Model
	{
	public:
		Model();
		~Model();
		void Init();
		Registry& GetRegistry() { return mRegistry; }
		Registry& GetPrivateScope() { return mPrivate; }
		Content& GetContent() { return mContent; }
		const Registry& GetRegistry() const { return mRegistry; }
		const Registry& GetPrivateScope() const { return mPrivate; }
		const Content& GetContent() const { return mContent; }
		vl::Object& GetType(const std::string& typeName);
		bool Load(const std::string& filePath);
		bool IsLoaded() const;
		bool Store(const std::string& filePath = "", const vl::CnvParams& params = vl::CnvParams());
		std::string JSONStr(const vl::CnvParams& params = vl::CnvParams());
		std::string GetTypeId(const vl::Object& obj) const;
		bool IsType(const vl::Object& obj) const;
		const vl::Object& GetData();
		void Clear(bool recursive = false);
		std::string DataStr(bool pretty = true) const;
		inline const vl::VarNodeRegistry& GetVarNodeRegistry() const {
			return mVarNodeRegistry;
		}
		inline const TypeResolver& GetTypeResolver() const {
			return mTypeResolver;
		}

	protected:
		vl::Object mData;
		Registry mRegistry;
		Registry mPrivate;
		Content mContent;
		vl::VarNodeRegistry mVarNodeRegistry;
		TypeResolver mTypeResolver;
		bool mIsLoaded = false;
		struct StoreState
		{
		public:
			StoreState() = default;
			StoreState(const std::string& __path, const vl::CnvParams& __params)
				: m_path(__path)
				, m_params(std::make_unique<vl::CnvParams>(__params))
			{}
			
			inline const std::string& GetPath() const { return m_path; }
			inline const vl::CnvParams* GetParams() const { return m_params.get(); }
			inline void SetPath(const std::string& path) { m_path = path; }
			inline const vl::CnvParams* SetParams(const vl::CnvParams& params) {
				m_params = std::make_unique<vl::CnvParams>(params);
			}
			inline void Reset() {
				m_path = "";
				m_params.reset();
			}
			
		private:
			std::string m_path;
			std::unique_ptr<vl::CnvParams> m_params;
		};
		StoreState mStoreState; // Set after every Store or Load operation
		
	};
}
