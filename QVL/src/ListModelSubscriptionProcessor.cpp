#include "ListModelSubscriptionProcessor.h"
#include "vl.h"
#include "VLListModel.h"
#include "VLObjectVarModel.h"
#include "PropPath.h"

#include <utils/log.h>
#ifdef LOG_ON
	#include <QDebug>
	#include "DMBModel.h"
	#include "VLUtility.h"
#endif
LOG_TITLE("ListModelSubscriptionProcessor")
LOG_STREAM([]{return qDebug();})
SET_LOCAL_LOG_LEVEL(debug)

namespace dmb
{
	void ListModelSubscriptionProcessor::doUpdateModel(int index)
	{
		auto& l = context().localData();
		if (auto& m = getOwner().getModelAt(index))
		{
			if (l.Has("oldType") && int(m->getData().GetType()) != l.Get("oldType").as<vl::Number>().Val())
				emit m->typeChanged();
			else
			{
				LOCAL_DEBUG("a call from doUpdatetModel([" << index << "]): " << m.get() << "->valueChanged()")
				emit m->valueChanged();
			}
			l.Set("modelExists", true);
		}
	}

	void ListModelSubscriptionProcessor::storeOldType(int index)
	{
		auto& c = context().localData();
		auto oldType = getOwner().getData(index).GetType();
		c.Set("oldType", int(oldType));
	}

	NotifContext &ListModelSubscriptionProcessor::context()
	{
		static NotifContext emptyProcess;
		return mStack.empty() ? emptyProcess : mStack.back();
	}


	VLListModel &ListModelSubscriptionProcessor::getOwner() {
		return *mOwner->as<vl::List>();
	}

	// "Add" processor functional
	void ListModelSubscriptionProcessor::onAdd()
	{
		auto& c = context();
		auto& o = c.notifData();
		auto index = o.Get("add").as<vl::Number>().Val();
		if (o.Has("before"))
			onBeforeAdd(index);
		else if (o.Has("after"))
			onAfterAdd(index);
	}

	// Before add
	void ListModelSubscriptionProcessor::onBeforeAdd(int index)
	{
		if (applyAdd(index))
			onApplyAdd(index);
	}

	bool ListModelSubscriptionProcessor::applyAdd(int index)
	{
		return context().path().empty();
	}

	void ListModelSubscriptionProcessor::onApplyAdd(int index)
	{
		auto& c = context();
		auto& l = context().localData();
		getOwner().beginInsertNewRow();
		l.Set("apply", true);
	}

	// After add
	void ListModelSubscriptionProcessor::onAfterAdd(int index)
	{
		// Load the model by default
		if (getOwner().getListStorage().size() > index)
			getOwner().loadElementModel(index);
		getOwner().endInsertRows();
	}

	// "Set" processor functional
	void ListModelSubscriptionProcessor::onSet()
	{
		auto& c = context();
		auto& o = c.notifData();
		auto index = o.Get("set").as<vl::Number>().Val();
		if (o.Has("before"))
			onBeforeSet(index);
		else if (o.Has("after"))
			onAfterSet(index);
	}

	// Before set
	void ListModelSubscriptionProcessor::onBeforeSet(int index)
	{
		auto& c = context();
		auto& l = c.localData();
		if (applySet(index))
		{
			storeOldType(index);
			l.Set("apply", true);
		}
	}

	bool ListModelSubscriptionProcessor::applySet(int index)
	{
		return context().path().empty();
	}

	void ListModelSubscriptionProcessor::onApplySet(int index)
	{
		doUpdateModel(index);
	}

	// After set
	void ListModelSubscriptionProcessor::onAfterSet(int index)
	{
		onApplySet(index);
	}

	// "Remove" processor functional
	void ListModelSubscriptionProcessor::onRemove()
	{
		auto& c = context();
		auto& o = c.notifData();
		auto index = o.Get("remove").as<vl::Number>().Val();
		if (o.Has("before"))
			onBeforeRemove(index);
		else if (o.Has("after"))
			onAfterRemove(index);
	}

	// Before remove
	void ListModelSubscriptionProcessor::onBeforeRemove(int index)
	{
		auto& c = context();
		auto& l = c.localData();
		if (applyRemove(index))
			onApplyRemove(index);
	}

	bool ListModelSubscriptionProcessor::applyRemove(int index)
	{
		return context().path().empty();
	}

	void ListModelSubscriptionProcessor::onApplyRemove(int index)
	{
		auto& c = context();
		auto& o = c.notifData();
		auto& l = c.localData();
		// Check if there is such element in the storage
		// and only then call begin/end RemoveRows
		if (index >= 0 && index < getOwner().dataSize())
		{
			getOwner().beginRemoveRows(QModelIndex(), index, index);
			// Store a flag for the "after" update
			l.Set("indexFromStorage", index);
			l.Set("apply", true);
		}
	}

	// After remove
	void ListModelSubscriptionProcessor::onAfterRemove(int index)
	{
		doRemove(index);
		getOwner().endRemoveRows();
			// TODO: experimental
//		emit getOwner().getParentModel()->propRemoved(id.c_str());
	}

	bool ListModelSubscriptionProcessor::doRemove(int index)
	{
		auto& c = context();
		auto& l = c.notifData();
		if (!getOwner().getListStorage().remove(index))
			return false;
		l.Set("removedFromStorage", true);
		return true;
	}
}
