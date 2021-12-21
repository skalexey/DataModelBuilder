#ifndef OBJECTPARAMETER_H
#define OBJECTPARAMETER_H
#include <QObject>
#include <QString>

class ObjectParameter : public QObject
{
    Q_OBJECT
public:
    enum Type {
        TypeString,
        TypeImage,
        TypeFlag,
        TypeObject,
        TypeListOfObjects
    };
    Q_ENUM (Type)

    static QVariantList typeList;

private:
    static QVariantList composeTypeList();

public:
    ObjectParameter(const QString& name, const Type& type)
        : name(name), type(type) {}
    ObjectParameter() = default;
    QString name;
    Type type;
};
Q_DECLARE_METATYPE (ObjectParameter)
#endif // OBJECTPARAMETER_H
