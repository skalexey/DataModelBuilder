#pragma once
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "vl_fwd.h"
#include <utils/patterns/Observer.h>

namespace vl
{
	// VarTreeNode
	class VarTreeNode;
	typedef std::shared_ptr<VarTreeNode> VarTreeNodePtr;
	class ObjectTreeNode;
	typedef std::shared_ptr<ObjectTreeNode> ObjectTreeNodePtr;
	class ListTreeNode;
	typedef std::shared_ptr<ListTreeNode> ListTreeNodePtr;
	class VarNodeRegistry;
	
	class VarTreeNode : public Observer {
		friend class VarNodeRegistry;
		friend class ListTreeNode;
		friend class ObjectTreeNode;

	public:
		VarTreeNode(VarNodeRegistry& registry, vl::AbstractVar* data, VarTreeNode* parent);
		virtual ~VarTreeNode();
		const std::string& GetId() const;
		int GetIndex() const;
		std::string GetPath() const;
		const VarNodeRegistry& GetRegistry() const;
		virtual bool IsList() const;
		virtual bool IsObject() const;
		virtual ObjectTreeNode* AsObject();
		virtual const ObjectTreeNode* AsObject() const;
		virtual ListTreeNode* AsList();
		virtual const ListTreeNode* AsList() const;
		void Update(Observable* sender, vl::VarPtr info) override;
		virtual std::size_t ChildCount() const;
		const vl::Var* GetData() const {
			return mData;
		}
		// Needed because of non-virtuality of std::shared_from_this()
		virtual VarTreeNodePtr SharedFromThis();
		bool IsRoot() const {
			return mParent == nullptr;
		}
		const VarTreeNode* GetParent() const {
			return mParent;
		}
		const void* DataPtr() const {
			return mDataPtr;
		}

	protected:
		bool ForeachChildren(const std::function<bool(VarTreeNode&)>& pred);
		void SetParent(VarTreeNode* newParent) {
			mParent = newParent;
		}
	protected:
		vl::AbstractVar* mData = nullptr;
		const void* mDataPtr = nullptr;
		VarTreeNode* mParent = nullptr;
		VarNodeRegistry& mRegistry;
		bool mToBeReplaced = false;
	};
	

	// ObjectTreeNode
	class ObjectTreeNode : public VarTreeNode, public std::enable_shared_from_this<ObjectTreeNode> {
		typedef VarTreeNode Base;
	public:
		ObjectTreeNode(VarNodeRegistry& registry, vl::AbstractVar* data, VarTreeNode* parent);
		~ObjectTreeNode();
		void Update(Observable* sender, vl::VarPtr info) override;
		bool Has(const std::string& childId) const;
		void Set(const std::string& childId, const VarTreeNodePtr& node);
		const VarTreeNodePtr& Get(const std::string& childId) const;
		std::size_t ChildCount() const override;
		ObjectTreeNode* AsObject() override;
		const ObjectTreeNode* AsObject() const override;
		bool IsObject() const override;
		bool Remove(const std::string& childId);
		VarTreeNodePtr SharedFromThis() override;
		ObjectTreeNodePtr ObjectSharedFromThis();

	protected:
		bool ForeachChildren(const std::function<bool(VarTreeNode&)>& pred);

	private:
		std::unordered_map<std::string, VarTreeNodePtr> mChildren;
	};
	typedef std::shared_ptr<ObjectTreeNode> ObjectTreeNodePtr;

	// ListTreeNode
	class ListTreeNode : public VarTreeNode, public std::enable_shared_from_this<ListTreeNode> {
		typedef VarTreeNode Base;
	public:
		ListTreeNode(VarNodeRegistry& registry, vl::AbstractVar* data, VarTreeNode* parent);
		~ListTreeNode();
		void Update(Observable* sender, vl::VarPtr info) override;
		std::size_t ChildCount() const override {
			return mChildren.size();
		}
		void Set(int index, const VarTreeNodePtr& node);
		void Add(const VarTreeNodePtr& node);
		void Resize(std::size_t newSize) {
			mChildren.resize(newSize);
		}
		const VarTreeNodePtr& At(std::size_t index) const;
		bool Clear(int index);
		ListTreeNode* AsList() override;
		bool IsList() const override;
		const ListTreeNode* AsList() const override;
		void Remove(int index);
		VarTreeNodePtr SharedFromThis() override;
		ListTreeNodePtr ListSharedFromThis();

	protected:
		bool ForeachChildren(const std::function<bool(VarTreeNode&)>& pred);
	private:
		std::vector<VarTreeNodePtr> mChildren;
	};
	typedef std::shared_ptr<ListTreeNode> ListTreeNodePtr;

	// VarNodeRegistry
	class VarNodeRegistry {
		// The tree in the registry consists of these classes:
		friend class VarTreeNode;
		friend class ObjectTreeNode;
		friend class ListTreeNode;

	public:
		// Initialize interface
		VarNodeRegistry(vl::AbstractVar* rootData);

	public:
		// Public interface
		const std::string& GetId(const VarTreeNode* node) const;
		int GetIndex(const VarTreeNode* node) const;
		const VarTreeNode* GetNode(const vl::Var& varPtr) const;
		const VarTreeNode* GetNode(const std::string& path) const;
		VarTreeNode& GetTree() {
			return mTree;
		}

	protected:
		// Protected interface
		VarTreeNodePtr CreateNode(vl::Var& data, VarTreeNode* parent);
		VarTreeNodePtr CreateNamedNode(const std::string& nodeId, vl::Var& data, VarTreeNode* parent);
		VarTreeNodePtr CreateIndexedNode(int index, vl::Var& data, VarTreeNode* parent);
		bool RemoveNode(const VarTreeNode* node);

	private:
		// Data
		std::unordered_multimap<const void*, VarTreeNode*> mVarToNode;
		std::unordered_map<const VarTreeNode*, std::string> mNamedNodes;
		std::unordered_map<const VarTreeNode*, int> mIndexedNodes;
		ObjectTreeNode mTree; // Reflects the root vl::Object
	};
}