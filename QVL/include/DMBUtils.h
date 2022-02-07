#ifndef DMBUTILS_H
#define DMBUTILS_H

#include <functional>
#include <QtQml/qqml.h>
#include <QObject>
#include "ModelsFwd.h"

namespace dmb
{
	class Utils : public QObject
	{
		Q_OBJECT
		QML_NAMED_ELEMENT(Utils)
		QML_SINGLETON

	public:
		explicit Utils(QObject *parent = nullptr);
	};
}

Q_DECLARE_METATYPE(dmb::Utils)
//Q_DECLARE_TYPEINFO(dmb::Utils, Q_HAS_ATTACHED_PROPERTIES)
#endif // DMBUTILS_H
