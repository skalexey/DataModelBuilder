#include "VLBackwardTraversable.h"
#include "Log.h"
#ifdef LOG_ON
	#include "Utils.h"
#endif
#include "vl.h"

namespace
{
	const std::string emptyString;
	const vl::VarTreeNodePtr nullTreeNodePtr = nullptr;
}

namespace vl
{

	vl::VarTreeNode::VarTreeNode(vl::VarNodeRegistry& registry, vl::AbstractVar* data, VarTreeNode* parent)
		: mRegistry(registry)
		, mData(data)
		, mParent(parent)
	{
		if (mData)
			mDataPtr = mData->Data();
	}

	VarTreeNode::~VarTreeNode()
	{
		mRegistry.RemoveNode(this);
		LOG_VERBOSE(Utils::FormatStr("~VarTreeNode() %p", this));
	}

	const std::string& vl::VarTreeNode::GetId() const
	{
		if (!mParent)
			return emptyString;
		if (!mData)
		{
			LOG_WARNING("An attempt to get id of an unitialized VarTreeNode");
			return emptyString;
		}
		if (mParent->IsObject())
			return mRegistry.GetId(this);
		else
			return emptyString;
	}

	int vl::VarTreeNode::GetIndex() const
	{
		if (!mParent)
			return -2;
		if (!mData)
		{
			LOG_WARNING("An attempt to get index of an unitialized VarTreeNode");
			return -3;
		}
		if (mParent->IsList())
			return mRegistry.GetIndex(this);
		else
			return -1;
	}

	std::string vl::VarTreeNode::GetPath() const
	{
		if (!mParent)
			return "";
		std::string id = GetId();
		if (id.empty())
		{
			auto index = GetIndex();
			if (index >= 0)
				id = Utils::FormatStr("[%d]", index);
			else
			{
				LOG_ERROR("Can't get id of a VarTreeNode with a parent. Maybe the parent has a wrong type or the registry has a wrong data");
				return "";
			}
		}
		if (mParent->IsRoot())
			return id;
		else
			return mParent->GetPath() + "." + id;
	}

	const vl::VarNodeRegistry& vl::VarTreeNode::GetRegistry() const
	{
		return mRegistry;
	}

	bool vl::VarTreeNode::IsList() const
	{
		return false;
	}

	bool vl::VarTreeNode::IsObject() const
	{
		return false;
	}

	vl::ObjectTreeNode* vl::VarTreeNode::AsObject()
	{
		return nullptr;
	}

	const vl::ObjectTreeNode* vl::VarTreeNode::AsObject() const
	{
		return nullptr;
	}

	vl::ListTreeNode* vl::VarTreeNode::AsList()
	{
		return nullptr;
	}

	const vl::ListTreeNode* vl::VarTreeNode::AsList() const
	{
		return nullptr;
	}

	void vl::VarTreeNode::Update(vl::VarPtr info)
	{
		// Nothing to do in the base class
	}

	int vl::VarTreeNode::ChildCount() const
	{
		// By default container with no children
		return 0;
	}

	vl::VarTreeNodePtr VarTreeNode::SharedFromThis()
	{
		return nullptr;
	}

	bool vl::VarTreeNode::ForeachChildren(const std::function<bool(VarTreeNode&)>& pred)
	{
		return false;
	}

	vl::VarNodeRegistry::VarNodeRegistry(vl::AbstractVar* rootData)
		: mTree(*this, rootData, nullptr)
	{
	}

	const std::string& vl::VarNodeRegistry::GetId(const VarTreeNode* node) const
	{
		auto it = mNamedNodes.find(node);
		if (it != mNamedNodes.end())
			return it->second;
		return emptyString;
	}

	int vl::VarNodeRegistry::GetIndex(const VarTreeNode* node) const
	{
		auto it = mIndexedNodes.find(node);
		if (it != mIndexedNodes.end())
			return it->second;
		return -1;
	}

	vl::VarTreeNodePtr vl::VarNodeRegistry::CreateNode(vl::Var& data, VarTreeNode* parent)
	{
		if (data.IsObject())
		{
			auto ptr = std::make_shared<ObjectTreeNode>(*this, &data, parent);
			data.AsObject().ForeachProp([&](const std::string& propName, vl::Var& value) {
				ptr->Set(propName, CreateNamedNode(propName, value, ptr.get()));
				return true;
			});
			return ptr;
		}
		else if (data.IsList())
		{
			auto ptr = std::make_shared<ListTreeNode>(*this, &data, parent);
			auto& list = data.AsList();
			auto listSize = list.Size();
			ptr->Resize(listSize);
			for (int i = 0; i < listSize; i++)
				ptr->Set(i, CreateIndexedNode(i, list.At(i), ptr.get()));
			return ptr;
		}
		else
			return std::make_shared<VarTreeNode>(*this, &data, parent);
	}

	bool VarNodeRegistry::RemoveNode(const VarTreeNode* node)
	{
		bool result = false;
		auto range = mVarToNode.equal_range(node->DataPtr());
		if (range.first != range.second)
			for (auto it = range.first; it != range.second; ++it)
				if (it->second == node)
				{
					mVarToNode.erase(it);
					result = true;
					break;
				}
		if (node->IsObject())
			result &= (mNamedNodes.erase(node) > 0);
		else if (node->IsList())
			result &= (mIndexedNodes.erase(node) > 0);
		return result;
	}

	vl::VarTreeNodePtr vl::VarNodeRegistry::CreateNamedNode(const std::string& nodeId, vl::Var& data, VarTreeNode* parent)
	{
		if (data.Data() == nullptr)
			return nullptr;
		auto ptr = CreateNode(data, parent);
		VarTreeNode* n = ptr.get();
		auto& currentId = ptr->GetId();
		mNamedNodes.emplace(n, nodeId);
		mVarToNode.emplace(data.Data(), n);
		return ptr;
	}

	vl::VarTreeNodePtr vl::VarNodeRegistry::CreateIndexedNode(int index, vl::Var& data, VarTreeNode* parent)
	{
		auto ptr = CreateNode(data, parent);
		VarTreeNode* n = ptr.get();
		mIndexedNodes.emplace(n, index);
		mVarToNode.emplace(data.Data(), n);
		return ptr;
	}

	const vl::VarTreeNode* vl::VarNodeRegistry::GetNode(const vl::Var& varPtr) const
	{
		auto range = mVarToNode.equal_range(varPtr.Data());
		for (auto it = range.first; it != range.second; ++it)
			if (it->second->GetId() == "proto")
				continue;
			else
				return it->second;
		return nullptr;
	}

	const vl::VarTreeNode* vl::VarNodeRegistry::GetNode(const std::string& path) const
	{
		auto parseIndex = [](const std::string& id) {
			if (id.size() < 3)
				return -1;
			return std::atoi(id.substr(1, id.size() - 2).c_str());
		};
		auto dotPos = std::string::npos;
		size_t cursor = 0;
		std::string lastId;
		VarTreeNodePtr node = nullTreeNodePtr;
		do {
			dotPos = path.find_first_of('.', cursor);
			auto id = dotPos == std::string::npos ? 
				path.substr(cursor) : path.substr(cursor, dotPos - cursor);
			auto n = node ? node.get() : &mTree;
			if (auto o = n->AsObject())
				node = o->Get(id);
			else if (auto l = n->AsList())
			{
				auto index = parseIndex(id);
				if (index >= 0 && index < l->ChildCount())
					node = l->At(index);
				else
				{
					LOG_ERROR(Utils::FormatStr("Wrong index %d in the path '%s' used for node '%s'", index, path.c_str(), lastId.c_str()));
					return nullptr;
				}
			}
			else
			{
				LOG_ERROR(Utils::FormatStr("Wrong container type during parsing the node path '%s'", path.c_str()));
				return nullptr;
			}
			cursor = dotPos + 1;
			lastId = id;

		} while (dotPos != std::string::npos && node != nullptr);
		return node.get();
	}

	vl::ObjectTreeNode::ObjectTreeNode(VarNodeRegistry& registry, vl::AbstractVar* data, VarTreeNode* parent)
		: Base(registry, data, parent)
	{
		LOG_VERBOSE(Utils::FormatStr("Create ObjectTreeNode %p for data %p with parent %p", this, data, parent));

		if (mData)
			mData->AsObject().Attach(this);
	}

	ObjectTreeNode::~ObjectTreeNode()
	{
		mRegistry.RemoveNode(this);
	}

	void vl::ObjectTreeNode::Update(vl::VarPtr info)
	{
		if (!info->IsString() && !info->IsObject())
		{
			LOG_WARNING("Wrong update info type passed to ObjectTreeNode observer");
			return;
		}
		if (info->IsString())
		{
			if (!mData)
			{
				LOG_WARNING("Update an unitialized ObjectTreeNode");
				return;
			}
			auto& obj = mData->AsObject();
			auto& childName = info->AsString().Val();
			
			//if (childName == "proto")
			//	return; // Ignore the proto as it has been already created

			if (!Has(childName))
			{
				if (obj.Has(childName))
					Set(childName, mRegistry.CreateNamedNode(childName, mData->AsObject().Get(childName), this));
				else
					LOG_WARNING("Unsupported update received by ObjectTreeNode [1]");
			}
			else // Has(childName)
			{
				if (!obj.Has(childName))
					Remove(childName);
				else
					//if (mData->GetType() != obj.Get(childName).GetType())
					// Always replace
					{
						Remove(childName);
						Set(childName, mRegistry.CreateNamedNode(childName, mData->AsObject().Get(childName), this));
					}
			}
		}
	}

	vl::ListTreeNode::ListTreeNode(VarNodeRegistry& registry, vl::AbstractVar* data, VarTreeNode* parent)
		: Base(registry, data, parent)
	{
		LOG_VERBOSE(Utils::FormatStr("Create ListTreeNode %p for data %p with parent %p", this, data, parent));

		if (mData)
			mData->AsList().Attach(this);
	}

	ListTreeNode::~ListTreeNode()
	{
		mRegistry.RemoveNode(this);
	}

	void vl::ListTreeNode::Update(vl::VarPtr info)
	{
		if (!info->IsNumber() && !info->IsObject())
		{
			LOG_WARNING("Wrong update info type passed to ListTreeNode observer");
			return;
		}
		if (info->IsObject())
		{
			if (!mData)
			{
				LOG_WARNING("Update an unitialized ListTreeNode");
				return;
			}
			auto& list = mData->AsList();
			auto listSize = list.Size();
			auto& infoObj = info->AsObject();
			auto index = infoObj.Get("index").AsNumber().Val();
			auto indexBefore = infoObj.Has("indexBefore") ? infoObj.Get("indexBefore").AsNumber().Val() : -1;
			auto count = ChildCount();
			if (indexBefore >= 0)
				mChildren.resize(count = indexBefore);
			if (index >= count) // No child
			{
				if (index < listSize)
				{
					mChildren.resize(listSize);
					for (int i = count; i < listSize; i++)
						Set(i, mRegistry.CreateIndexedNode(i, mData->AsList().At(i), this));
				}
				else
					LOG_WARNING("Unsupported update received by ListTreeNode [2]");
			}
			else // index < count // Child exists
			{
				if (index >= listSize) // No data
					for (int i = listSize; i < count; i++)
						Remove(count - 1 - i);
				else // index < listSize // Data exists
					{
						Clear(index);
						Set(index, mRegistry.CreateIndexedNode(index, mData->AsList().At(index), this));
					}
			}
		}
		else
			LOG_WARNING("Unsupported update received by ListTreeNode [1]");
	}

	int vl::ListTreeNode::ChildCount() const
	{
		return mChildren.size();
	}

	bool vl::ObjectTreeNode::Has(const std::string& childId) const
	{
		return mChildren.find(childId) != mChildren.end();
	}

	void vl::ObjectTreeNode::Set(const std::string& childId, const VarTreeNodePtr& node)
	{
		mChildren[childId] = node;
	}

	const vl::VarTreeNodePtr& vl::ObjectTreeNode::Get(const std::string& childId) const
	{
		auto it = mChildren.find(childId);
		if (it != mChildren.end())
			return it->second;
		return nullTreeNodePtr;
	}

	bool vl::ObjectTreeNode::Remove(const std::string& childId)
	{
		LOG_VERBOSE(Utils::FormatStr("Erase child '%s' from %p", childId.c_str(), this));
		auto it = mChildren.find(childId);
		if (it != mChildren.end())
		{
			mChildren.erase(it);
			return true;
		}
		else
			LOG_WARNING(Utils::FormatStr("Attempt to remove non-existent child '%s' of an object '%s'", childId.c_str(), GetId().c_str()));
		return false;
	}

	VarTreeNodePtr ObjectTreeNode::SharedFromThis()
	{
		return shared_from_this();
	}

	ObjectTreeNodePtr ObjectTreeNode::ObjectSharedFromThis()
	{
		return shared_from_this();
	}

	int vl::ObjectTreeNode::ChildCount() const
	{
		return mChildren.size();
	}

	vl::ObjectTreeNode* vl::ObjectTreeNode::AsObject()
	{
		return this;
	}

	const vl::ObjectTreeNode* vl::ObjectTreeNode::AsObject() const
	{
		return this;
	}

	bool ObjectTreeNode::IsObject() const
	{
		return true;
	}

	bool vl::ObjectTreeNode::ForeachChildren(const std::function<bool(VarTreeNode&)>& pred)
	{
		for (auto& [id, child] : mChildren)
			if (!pred(*child))
				return false;
		return true;
	}

	void vl::ListTreeNode::Set(int index, const VarTreeNodePtr& node)
	{
		mChildren[index] = node;
	}

	void vl::ListTreeNode::Remove(int index)
	{
		LOG_VERBOSE(Utils::FormatStr("Erase child '%d' from %p", index, this));
		mChildren.erase(mChildren.begin() + index);
	}

	VarTreeNodePtr ListTreeNode::SharedFromThis()
	{
		return std::enable_shared_from_this<ListTreeNode>::shared_from_this();
	}

	vl::ListTreeNodePtr ListTreeNode::ListSharedFromThis()
	{
		return std::enable_shared_from_this<ListTreeNode>::shared_from_this();
	}

	const vl::VarTreeNodePtr& vl::ListTreeNode::At(int index) const
	{
		return mChildren[index];
	}

	bool vl::ListTreeNode::Clear(int index)
	{
		mChildren[index] = nullptr;
		return true;
	}

	vl::ListTreeNode* vl::ListTreeNode::AsList()
	{
		return this;
	}

	const vl::ListTreeNode* vl::ListTreeNode::AsList() const
	{
		return this;
	}

	bool ListTreeNode::IsList() const
	{
		return true;
	}

	bool vl::ListTreeNode::ForeachChildren(const std::function<bool(VarTreeNode&)>& pred)
	{
		for (auto& child : mChildren)
			if (!pred(*child))
				return false;
		return true;
	}
}
