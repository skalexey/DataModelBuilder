#include <QQmlApplicationEngine>
#include "ObjectProperty.h"
#include "vl.h"
#include "VLVarModel.h"

namespace dmb
{
	ObjectProperty::ObjectProperty(QObject *parent)
		: QObject{parent}
	{}

	const QVariantList &ObjectProperty::vlTypeModel()
	{
		if (mTypeList.empty())
			mTypeList = composeTypeList();
		return mTypeList;
	}

	const QVariantList &ObjectProperty::vlTypeModelDetailed()
	{
		if (mTypeMap.empty())
			mTypeMap = composeTypeMap();
		return mTypeMap;
	}

	// ======= Begin of Static interface =======
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

	ObjectProperty::Type ObjectProperty::typeFromQVariant(const QVariant &data)
	{
		switch (data.userType())
		{
		case QMetaType::QString:
			return Type::String;
		case QMetaType::Int:
		case QMetaType::Double:
		case QMetaType::Float:
			return Type::Int;
		case QMetaType::Bool:
			return Type::Bool;
		default:
			return Type::Object;
		}
	}

	vl::VarPtr ObjectProperty::makeVarFromData(const QVariant &data, vl::Type type)
	{
		if (type != vl::Type::Count)
		{
			switch (type)
			{
			case vl::Type::Bool:
				return vl::MakePtr(data.toBool());
			case vl::Type::Number:
				return vl::MakePtr(data.toInt());
			case vl::Type::String:
				return vl::MakePtr(data.toString().toStdString());
			case vl::Type::Object:
			{
				if (data.canConvert<VLVarModel*>())
					return vl::MakePtr(qvariant_cast<VLVarModel*>(data)->getData().AsObject());
				break;
			}
			case vl::Type::List:
			{
				if (data.canConvert<VLVarModel*>())
					return vl::MakePtr(qvariant_cast<VLVarModel*>(data)->getData().AsList());
				break;
			}
			case vl::Type::Null:
				return vl::MakePtr();
			default:
				return vl::MakePtr();
			}
			return vl::MakePtr();
		}
		else
		{
			switch (data.userType())
			{
			case QMetaType::QString:
				return vl::MakePtr(data.toString().toStdString());
			case QMetaType::Int:
			case QMetaType::Double:
			case QMetaType::Float:
				return vl::MakePtr(data.toInt());
			case QMetaType::Bool:
				return vl::MakePtr(data.toBool());
			default:
				return vl::MakePtr(data.toString().toStdString());
			}
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
