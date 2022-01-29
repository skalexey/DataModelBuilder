#include <QQmlApplicationEngine>
#include "ObjectProperty.h"
#include "vl.h"

namespace dmb
{
	ObjectProperty::ObjectProperty(QObject *parent)
		: QObject{parent}
	{}

	// ======= Begin of Static interface =======
	QVariantList ObjectProperty::typeList = composeTypeList();
	QVariantList ObjectProperty::typeMap = composeTypeMap();

	ObjectProperty::Type ObjectProperty::ConvertVLType(const vl::Var &var)
	{
		if (var.IsBool())
			return Type::Bool;
		else if (var.IsNumber())
			return Type::Int;
		else if (var.IsString())
			return Type::String;
		else if (var.IsList())
			return Type::List;
		else if (var.IsObject())
			return Type::Object;
		return Type::Null;
	}

	const char* ObjectProperty::GetTypeStr(const vl::Var &var)
	{
		auto& metaObj = ObjectProperty::staticMetaObject;
		return metaObj.enumerator(metaObj.indexOfEnumerator("Type")).key(ConvertVLType(var));;
	}

	vl::VarPtr ObjectProperty::MakeVarPtr(Type type)
	{
		switch (type) {
			case Type::String:
				return std::make_shared<vl::StringVar>();
			case Type::Int:
				return std::make_shared<vl::NumberVar>();
			case Type::Bool:
				return std::make_shared<vl::BoolVar>();
			case Type::Object:
				return std::make_shared<vl::ObjectVar>();
			case Type::List:
				return std::make_shared<vl::ListVar>();
			case Type::Null:
			default:
				return std::make_shared<vl::NullVar>();
		}
	}

	QVariantList ObjectProperty::composeTypeList()
	{
		QVariantList list;
		auto& metaObj = ObjectProperty::staticMetaObject;
		auto enumIndex = metaObj.indexOfEnumerator("Type");
		auto en = metaObj.enumerator(enumIndex);
		auto count = en.keyCount();
		for (int i = 0; i < count; i++)
			list.append(QString(en.key(i)));
		return list;
	}

	QVariantList ObjectProperty::composeTypeMap()
	{
		QVariantList list;

		auto& metaObj = ObjectProperty::staticMetaObject;
		auto enumIndex = metaObj.indexOfEnumerator("Type");
		auto en = metaObj.enumerator(enumIndex);
		auto count = en.keyCount();
		for (int i = 0; i < count; i++)
		{
			QVariantMap map;
			map[en.key(i)] = i;
			list.append(map);
		}
		return list;
	}
	// ======= End of Static interface =======
}
