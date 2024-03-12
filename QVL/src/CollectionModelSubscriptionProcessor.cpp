#include <utility>
#include "CollectionModelSubscriptionProcessor.h"
#include "vl.h"
#include "VLCollectionModel.h"
#include "VLObjectVarModel.h"
#include "PropPath.h"

#include <utils/log.h>
#ifdef LOG_ON
	#include <QDebug>
	#include "DMBModel.h"
	#include "VLUtility.h"
#endif
LOG_TITLE("CollectionModelSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})
SET_LOCAL_LOG_LEVEL(debug)

namespace dmb
{
	bool CollectionModelSubscriptionProcessor::processUpdate(vl::Var& v, const std::string& path)
	{
		auto& o = v.as<vl::Object>();
		if (o.Has("propUpdate"))
		{
			auto& u = o.Get("propUpdate").as<vl::Object>();
			if (u.Has("data"))
				return processUpdate(u.Get("data"), (path.empty() ? "" : (path + ".")) + u.Get("id").as<vl::String>().Val());
			return false;
		}
		else
		{
			return updateContext(v, path);
		}
	}

	VLCollectionModel &CollectionModelSubscriptionProcessor::getOwner() {

		return *mOwner->asCollection();
	}

	VLListModelInterface &CollectionModelSubscriptionProcessor::getOwnerBase() {

		return *mOwner;
	}

	void CollectionModelSubscriptionProcessor::Update(vl::Observable* sender, vl::VarPtr info)
	{
		if (auto parent = std::as_const(getOwnerBase()).getParentModel())
			LOCAL_DEBUG("Update vl notification on object '" << parent->getStringId().c_str() << "' and collection " << this);

		LOCAL_DEBUG("msg: " << vl::VarToJSON(*info).c_str());

		if (!info->is<vl::Object>())
			return;
		auto& o = info->as<vl::Object>();

		auto& who = o.Get("who").as<vl::String>().Val();
		if (who != "vl")
		{
			LOCAL_ERROR("Unsupported update from '" << who.c_str() << "'");
			return;
		}

		if (!updateContext(o))
			return;

		// Now we have a context and a recognized command and can process it
		// Works until not return true
		processCommand();
	}

	bool CollectionModelSubscriptionProcessor::updateContext(vl::Var& info, const std::string& path)
	{
		if (!path.empty())
			if (!PropPath(path).isProtoPath())
				return false; // Ignore all no proto nested objects updates

		if (!info.is<vl::Object>())
			return false;

		auto& o = info.as<vl::Object>();

		// Proces update recursively until we reach the final command
		if (o.Has("propUpdate"))
			return processUpdate(o);

		std::string cmd = "";
		if (o.Has("add"))
			cmd = "add";
		if (o.Has("set"))
			cmd = "set";
		else if (o.Has("remove"))
			cmd = "remove";
		else if (o.Has("rename"))
			cmd = "rename";
		else if (o.Has("clear"))
			cmd = "clear";

		// Place a command to the stack or pop it from
		if (o.Has("before"))
		{
			mStack.emplace(cmd, path, info, mOnBeforeUpdate, mOnAfterUpdate);
			mOnBeforeUpdate = nullptr;
			mOnAfterUpdate = nullptr;
		}
		else if (o.Has("after"))
		{
			auto& c = mStack.back();
			if (c == NotifContext(cmd, path, info))
			{
				if (c.localData().Has("apply"))
					return true;
				// If have not got into the "after" case
				// then whatever check the cb before destruction of the context
				c.processCbAfterUpdate();
				mStack.pop();
				return false; // Ignore "after" processing
			}
			else
			{
				LOCAL_ERROR("Notifications order is broken. Will ignore command '" << cmd.c_str() << "' from path '" << path.c_str() << "'");
				return false;
			}
		}
		return true;
	}

	void CollectionModelSubscriptionProcessor::onNoRelativePath(const std::string& id)
	{
		if (auto parent = std::as_const(getOwnerBase()).getParentModel())
			LOCAL_ERROR("Can't find relative path for id '" << id.c_str() << "' in the object '" << parent->getStringId().c_str() );
		else
			LOCAL_ERROR("Can't find relative path for id '" << id.c_str() << "' because there is no parent attached to the collection model " << &mOwner);
	}

	void CollectionModelSubscriptionProcessor::doUpdateModel(const std::string& id)
	{
		auto& l = context().localData();
		if (auto& m = getOwner().getModel(id))
		{
			if (l.Has("oldType") && int(m->getData().GetType()) != l.Get("oldType").as<vl::Number>().Val())
				emit m->typeChanged();
			else
			{
				LOCAL_DEBUG("a call from doUpdatetModel('" << id.c_str() << "'): " << m.get() << "->valueChanged()")
				emit m->valueChanged();
			}
			l.Set("modelExists", true);
		}
	}

	void CollectionModelSubscriptionProcessor::storeOldType(const std::string& id)
	{
		auto& c = context().localData();
		auto oldType = getOwner().getData(id).GetType();
		c.Set("oldType", int(oldType));
	}

	void CollectionModelSubscriptionProcessor::processCommand()
	{
		auto& c = context();

		c.processCbBeforeUpdate();

		auto& cmd = c.cmd();
		if (cmd == "add")
			onAdd();
		if (cmd == "set")
			onSet();
		else if (cmd == "remove")
			onRemove();
		else if (cmd == "rename")
			onRename();
		else if (cmd == "clear")
			onClear();

		c.processCbAfterUpdate();
	}

	NotifContext &CollectionModelSubscriptionProcessor::context()
	{
		static NotifContext emptyProcess;
		return mStack.empty() ? emptyProcess : mStack.back();
	}

	// "Add" processor functional
	void CollectionModelSubscriptionProcessor::onAdd()
	{
		auto& c = context();
		auto& o = c.notifData();
		auto& id = o.Get("add").as<vl::String>().Val();
		if (o.Has("before"))
			onBeforeAdd(id);
		else if (o.Has("after"))
			onAfterAdd(id);
	}

	// Before add
	void CollectionModelSubscriptionProcessor::onBeforeAdd(const std::string &id)
	{
		if (applyAdd(id))
			onApplyAdd(id);
	}

	bool CollectionModelSubscriptionProcessor::applyAdd(const std::string& id)
	{
		return context().path().empty();
	}

	void CollectionModelSubscriptionProcessor::onApplyAdd(const std::string &id)
	{
		auto& c = context();
		auto& l = context().localData();
		getOwner().beginInsertNewRow();
		l.Set("apply", true);
	}

	// After add
	void CollectionModelSubscriptionProcessor::onAfterAdd(const std::string &id)
	{
		// Load the model by default
		if (!getOwner().getObjectStorage().has(id))
			getOwner().loadElementModel(id);
		getOwner().endInsertRows();
	}

	// "Set" processor functional
	void CollectionModelSubscriptionProcessor::onSet()
	{
		auto& c = context();
		auto& o = c.notifData();
		auto& id = o.Get("set").as<vl::String>().Val();
		if (o.Has("before"))
			onBeforeSet(id);
		else if (o.Has("after"))
			onAfterSet(id);
	}

	// Before set
	void CollectionModelSubscriptionProcessor::onBeforeSet(const std::string &id)
	{
		auto& c = context();
		auto& l = c.localData();
		if (applySet(id))
		{
			storeOldType(id);
			l.Set("apply", true);
		}
	}

	bool CollectionModelSubscriptionProcessor::applySet(const std::string& id)
	{
		return context().path().empty();
	}

	void CollectionModelSubscriptionProcessor::onApplySet(const std::string& id)
	{
		doUpdateModel(id);
	}

	// After set
	void CollectionModelSubscriptionProcessor::onAfterSet(const std::string &id)
	{
		onApplySet(id);
	}

	// "Remove" processor functional
	void CollectionModelSubscriptionProcessor::onRemove()
	{
		auto& c = context();
		auto& o = c.notifData();
		auto& id = o.Get("remove").as<vl::String>().Val();
		if (o.Has("before"))
			onBeforeRemove(id);
		else if (o.Has("after"))
			onAfterRemove(id);
	}

	// Before remove
	void CollectionModelSubscriptionProcessor::onBeforeRemove(const std::string &id)
	{
		auto& c = context();
		auto& l = c.localData();
		if (applyRemove(id))
			onApplyRemove(id);
	}

	bool CollectionModelSubscriptionProcessor::applyRemove(const std::string& id)
	{
		return context().path().empty();
	}

	void CollectionModelSubscriptionProcessor::onApplyRemove(const std::string& id)
	{
		auto& c = context();
		auto& o = c.notifData();
		auto& l = c.localData();
		// Check if there is such element in the storage
		// and only then call begin/end RemoveRows
		auto index = getOwner().getIndex(id);
		if (index >= 0 && index < getOwner().dataSize())
		{
			getOwner().beginRemoveRows(QModelIndex(), index, index);
			// Store a flag for the "after" update
			l.Set("indexFromStorage", index);
			l.Set("apply", true);
		}
	}

	// After remove
	void CollectionModelSubscriptionProcessor::onAfterRemove(const std::string &id)
	{
		doRemove(id);
		getOwner().endRemoveRows();
			// TODO: experimental
//		emit getOwner().getParentModel()->propRemoved(id.c_str());
	}

	bool CollectionModelSubscriptionProcessor::doRemove(const std::string& id)
	{
		auto& c = context();
		auto& l = c.notifData();
		if (!getOwner().getObjectStorage().remove(id))
			return false;
		l.Set("removedFromStorage", true);
		return true;
	}

	// "Rename" processor functional
	bool CollectionModelSubscriptionProcessor::applyRename(const std::string& id, const std::string& newId)
	{
		return context().path().empty();
	}

	bool CollectionModelSubscriptionProcessor::doRename(const std::string &id, const std::string &newId)
	{
		auto& c = context();
		auto& l = c.notifData();
		if (!getOwner().getObjectStorage().rename(id, newId))
			return false;
		l.Set("renamedInStorage", true);
		return true;
	}

	// Rename notif processor
	void CollectionModelSubscriptionProcessor::onRename()
	{
		auto& c = context();
		auto& o = c.notifData();
		auto& id = o.Get("rename").as<vl::String>().Val();
		auto& newId = o.Get("newId").as<vl::String>().Val();
		if (o.Has("before"))
			onBeforeRename(id, newId);
		else if (o.Has("after"))
			onAfterRename(id, newId);
	}

	// Before rename
	void CollectionModelSubscriptionProcessor::onBeforeRename(const std::string &id, const std::string& newId)
	{
		auto& c = context();
		auto& l = c.localData();
		if (applyRename(id, newId))
		{
			doRename(id, newId);
			l.Set("apply", true);
		}
	}

	// After rename
	void CollectionModelSubscriptionProcessor::onAfterRename(const std::string &id, const std::string &newId)
	{
		auto& c = context();
		auto& l = c.localData();
		if (auto& model = getOwner().getModel(newId))
		{
			// Call onNameChanged
			emit model->idChanged();
			l.Set("modelExists", true);
		}
	}

	// "Clear" processor functional
	void CollectionModelSubscriptionProcessor::onClear()
	{
		auto& c = context();
		auto& o = c.notifData();
		if (o.Has("before"))
			onBeforeClear();
		else if (o.Has("after"))
			onAfterClear();
	}

	void CollectionModelSubscriptionProcessor::onBeforeClear()
	{
		auto& c = context();
		auto& l = c.localData();
		auto sz = getOwnerBase().size();
		if (sz > 0)
		{
			l.Set("apply", sz);
			//getOwnerBase().beginRemoveRows(QModelIndex(), 0, sz - 1);
			getOwnerBase().clear();
		}
	}

	void CollectionModelSubscriptionProcessor::onAfterClear()
	{
		//if (o.Has("size"))
			//getOwnerBase().endRemoveRows();
	}
}
