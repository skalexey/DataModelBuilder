#ifndef OBJECTPARAMETERMODEL_H
#define OBJECTPARAMETERMODEL_H

#include <QtQml/qqml.h>
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
		Q_OBJECT
		QML_NAMED_ELEMENT(ObjectProperty)
		QML_SINGLETON

	public:
		explicit ObjectProperty(QObject *parent = nullptr);

		enum Type {
			String,
			Int,
			Bool,
			Object,
			List,
			Null
		};
		Q_ENUM (Type)

	public:
		// Properties
		Q_PROPERTY (const QVariantList& vlTypeModel READ vlTypeModel NOTIFY vlTypeModelChanged)
		Q_PROPERTY (const QVariantList& vlTypeModelDetailed READ vlTypeModelDetailed NOTIFY vlTypeModelDetailedChanged)
		QVariantList composeTypeList();
		QVariantList composeTypeMap();
		const QVariantList& vlTypeModel();
		const QVariantList& vlTypeModelDetailed();
		// ======= Static interface =======
		static Type ConvertVLType(const vl::Var& var);
		static const char* GetTypeStr(const vl::Var& var);
		static vl::VarPtr MakeVarPtr(Type type);

	signals:
		void vlTypeModelChanged();
		void vlTypeModelDetailedChanged();

	protected:
		QVariantList mTypeList;
		QVariantList mTypeMap;
	};
}

Q_DECLARE_METATYPE(dmb::ObjectProperty::Type);

#endif // OBJECTPARAMETERMODEL_H
