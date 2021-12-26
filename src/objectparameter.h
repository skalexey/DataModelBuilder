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
        : name(name), mType(type) {}
    ObjectParameter() = default;
    QString name;

    Q_PROPERTY (Type type READ type WRITE setType NOTIFY typeChanged)
    Type type() const;
    void setType(Type newType);

signals:
    void typeChanged();

private:
    Type mType = TypeString;
};
Q_DECLARE_METATYPE (ObjectParameter)
#endif // OBJECTPARAMETER_H
