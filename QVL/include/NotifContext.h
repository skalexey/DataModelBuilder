#ifndef NOTIFCONTEXT_H
#define NOTIFCONTEXT_H

#include <queue>
#include <string>
#include "vl_fwd.h"
#include "ModelsFwd.h"

namespace dmb
{
	class NotifContext
	{
	public:
		NotifContext() = default;
		NotifContext(
			const std::string& cmd, const std::string& path, const vl::Var& notifData
			, const BoolCbContext& onBeforeUpdate = nullptr
			, const BoolCbContext& onAfterUpdate = nullptr
		);

		vl::Object& localData();
		vl::Object& notifData();
		const vl::Object& getLocalData() const;
		const vl::Object& getNotifData() const;
		const std::string& cmd() {
			return mCmd;
		}
		const std::string& path() {
			return mPath;
		}

		bool operator == (const NotifContext& right) {
			return right.mPath == mPath && right.mCmd == mCmd;
		}
		void processCbBeforeUpdate() {
			if (mOnBeforeUpdate)
				if (mOnBeforeUpdate(*this))
					mOnBeforeUpdate = nullptr;
		}
		void processCbAfterUpdate() {
			if (mOnAfterUpdate)
				if (mOnAfterUpdate(*this))
					mOnAfterUpdate = nullptr;
		}

	protected:
		vl::VarPtr mLocalData = nullptr;
		vl::VarPtr mNotifData = nullptr;
		std::string mPath;
		std::string mCmd;
		BoolCbContext mOnBeforeUpdate;
		BoolCbContext mOnAfterUpdate;
	};
}

#endif // NOTIFCONTEXT_H
