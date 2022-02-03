#ifndef DMBUTILS_H
#define DMBUTILS_H

#include <functional>
#include <QObject>
#include "ModelsFwd.h"

namespace dmb
{
	class Utils : public QObject
	{
		Q_OBJECT
	public:
		static Utils& Instance();
		// Support inheritance
		template <typename T>
		void Choose() {
			mCreator = [] {
				return new T;
			};
		}
	protected:
		explicit Utils(QObject *parent = nullptr);

	protected:
		static Utils* mInstance;
		static std::function<Utils*()> mCreator;

	public:


	signals:

	};
}

Q_DECLARE_METATYPE(dmb::Utils)
//Q_DECLARE_TYPEINFO(dmb::Utils, Q_HAS_ATTACHED_PROPERTIES)
#endif // DMBUTILS_H
