#include "AllPropsSubscriptionProcessor.h"
#include "vl.h"
#include "VLObjectVarModel.h"

#include <utils/Log.h>
#ifdef LOG_ON
	#include <QDebug>
	#include "DMBModel.h"
#endif
LOG_TITLE("AllPropsSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})

namespace dmb
{
	// Add
	void AllPropsSubscriptionProcessor::onBeforeAdd(const std::string &id)
	{
		auto& c = context();
		auto& l = c.localData();
		auto& path = c.path();
		bool update = false;
		bool apply = false;
		apply = !getOwner().getData().Has(id);
		if (!apply)
		{
			if (auto relativePath = getOwner().getData().GetRelativePath(id))
				// Update if added to the more near level the the currently used
				update = path.size() <= relativePath->size();
			else
			{
				onNoRelativePath(id);
				update = true;
			}
		}
		else
			update = false;
		if (apply)
			onApplyAdd(id);
		else if (update)
		{
			storeOldType(id);
			l.Set("doUpdate", true);
			l.Set("apply", true);
		}
	}

	void AllPropsSubscriptionProcessor::onAfterAdd(const std::string &id)
	{
		auto& c = context();
		auto& l = c.localData();
		auto& path = c.path();
		if (l.Has("doUpdate"))
			doUpdateModel(id);
		else
			Base::onAfterAdd(id);
	}

	// Set
	bool AllPropsSubscriptionProcessor::applySet(const std::string& id)
	{
		auto& c = context();
		auto& path = c.path();
		if (!path.empty())
		{
			if (auto relativePath = getOwner().getData().GetRelativePath(id))
				return *relativePath == path;
			else
			{
				onNoRelativePath(id);
				return false;
			}
		}
		return true;
	}

	// Remove
	void AllPropsSubscriptionProcessor::onBeforeRemove(const std::string &id)
	{
		auto& c = context();
		auto& l = c.localData();
		auto& path = c.path();
		bool remove = false;
		bool update = false;
		auto& data = getOwner().getData();
		bool overridden = data.Overridden(id);
		// If we have more than one such prop in the proto chain
		// Then no need to remove it. Just decide update or not next
		remove = !overridden;
		update = false;
		if (overridden)
		{
			if (auto relativePath = data.GetRelativePath(id))
			{
				if (!path.empty())
					// If it is an update of the currently used
					update = *relativePath == path;
				else
					update = true;
			}
			else
			{
				onNoRelativePath(id);
				update = true;
			}
		}
		if (remove)
			onApplyRemove(id);
		else if (update)
		{
			storeOldType(id);
			l.Set("doUpdate", true);
			l.Set("apply", true);
		}
	}

	void AllPropsSubscriptionProcessor::onAfterRemove(const std::string &id)
	{
		auto& c = context().localData();
		if (c.Has("doUpdate"))
			doUpdateModel(id);
		else
			Base::onAfterRemove(id);
	}


	bool AllPropsSubscriptionProcessor::doRemove(const std::string& id)
	{
		return Base::doRemove(id);
	}

	// Custom Rename notif processor
	void AllPropsSubscriptionProcessor::onBeforeRename(const std::string& id, const std::string& newId)
	{
		auto& c = context();
		auto& l = c.localData();
		auto& path = c.path();
		auto& data = getOwner().getData();
		// If we got here then newId doesn't exist
		// in the owner of the renaming property
		bool oldOverridden = data.Overridden(id);
		bool newOverridden = data.Overridden(newId);
		// If we have more than one such prop in the proto chain
		// Then no need to remove it. Just decide update or not next
		bool removeOld = !oldOverridden;
		bool addNew = !data.Has(newId);
		bool rename = removeOld && addNew;
		bool updateNew = false;
		bool updateOld = false;
		if (rename)
		{
			// Rename in the storage list before the data is changed
			doRename(id, newId);
			l.Set("doRename", true);
			l.Set("apply", true);
		}
		else
		{
			if (!addNew)
			{
				if (auto relativePathOld = data.GetRelativePath(id))
				{
					if (auto relativePathNew = data.GetRelativePath(newId))
						updateNew = relativePathOld->size() <= relativePathNew->size();
					else
					{
						onNoRelativePath(newId);
						updateNew = true;
					}
				}
				else
				{
					onNoRelativePath(id);
					updateNew = true;
				}
			}
			if (!removeOld)
			{
				if (path.empty())
					updateOld = true;
				else
					if (auto relativePathOld = data.GetRelativePath(id))
						updateOld = *relativePathOld == path;
					else
						onNoRelativePath(id);
			}

			if (addNew)
			{
				getOwner().beginInsertNewRow();
				l.Set("doAddNew", true);
				l.Set("apply", true);
			}
			else if (updateNew)
			{
				l.Set("doUpdateNew", true);
				l.Set("apply", true);
				storeOldType(newId);
			}

			if (removeOld)
			{
				auto index = getOwner().getIndex(id);
				if (index >= 0)
				{
					getOwner().beginRemoveRows(QModelIndex(), index, index);
					getOwner().getObjectStorage().remove(id);
					l.Set("doRemoveOld", index);
					l.Set("apply", true);
				}
			}
			else if (updateOld)
			{
				l.Set("doUpdateOld", true);
				l.Set("apply", true);
				storeOldType(id);
			}
		}
	}


	// Custom onAfterRename method
	void AllPropsSubscriptionProcessor::onAfterRename(const std::string &id, const std::string &newId)
	{
		auto& c = context();
		auto& l = c.localData();
		auto& path = c.path();
		if (l.Has("doAddNew"))
		{
			if (!getOwner().getObjectStorage().get(newId))
				getOwner().loadElementModel(newId);
			else
				LOCAL_ERROR("Unsynchronized storage and data. On notification 'rename(" << id.c_str() << " to " << newId.c_str() << ")' the data for the new one has not been existed before renaming, but the model has");
			getOwner().endInsertRows();
		}
		else if (l.Has("doUpdateNew"))
			doUpdateModel(newId);
		if (l.Has("doUpdateOld"))
			doUpdateModel(id);
		else if (l.Has("doRemoveOld"))
			// Already removed in the "before" part
			getOwner().endRemoveRows();
		else if ("doRename")
		{	// Process "doRename" using default logic
			// Call onNameChanged
			Base::onAfterRename(id, newId);
		}
	}
}
