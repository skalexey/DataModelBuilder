#ifndef OBJECTPARAMETERMODEL_H
#define OBJECTPARAMETERMODEL_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QVariantList>
#include <QVariantMap>
#include "vl_fwd.h"

namespace dmb
{
	class ObjectProperty : public QObject
	{
	public:
		enum Type {
			String,
			Int,
			Bool,
			Object,
			List,
			Null
		};
		Q_ENUM (Type)
		// ======= Static interface =======
			static QVariantList typeList;
			static QVariantList typeMap;
			static QVariantList composeTypeList();
			static QVariantList composeTypeMap();
			static Type ConvertVLType(const vl::Var& var);
			static const char* GetTypeStr(const vl::Var& var);
			static vl::VarPtr MakeVarPtr(Type type);

	private:
		Q_OBJECT

	public:
		explicit ObjectProperty(QObject *parent = nullptr);
	};
}

Q_DECLARE_METATYPE(dmb::ObjectProperty::Type);

#endif // OBJECTPARAMETERMODEL_H
