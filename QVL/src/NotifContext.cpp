#include "NotifContext.h"
#include "vl.h"

namespace dmb
{
	NotifContext::NotifContext(
				const std::string &cmd, const std::string &path, const vl::Var& notifData
				, const BoolCbContext& onBeforeUpdate
				, const BoolCbContext& onAfterUpdate
			)
		: mCmd(cmd)
		, mPath(path)
		, mLocalData(vl::MakePtr(vl::Object()))
		, mNotifData(vl::MakePtr(notifData))
		, mOnBeforeUpdate(onBeforeUpdate)
		, mOnAfterUpdate(onAfterUpdate)
	{}

	vl::Object &NotifContext::localData()
	{
		return mLocalData->AsObject();
	}

	const vl::Object &NotifContext::getLocalData() const
	{
		return mLocalData->AsObject();
	}

	vl::Object &NotifContext::notifData()
	{
		return mNotifData->AsObject();
	}

	const vl::Object &NotifContext::getNotifData() const
	{
		return mNotifData->AsObject();
	}
}
