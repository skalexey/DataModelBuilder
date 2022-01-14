#ifndef OBJECT_H
#define OBJECT_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <memory>
#include "PropListModel.h"

class TypeListModel;

namespace vl
{
    class ObjectVar;
    typedef ObjectVar Object;
}

struct TypeModel : public QObject
{
    Q_OBJECT

public:
    explicit TypeModel(QObject* parent = nullptr);
    TypeModel(TypeListModel* modelPtr, int index);
    ~TypeModel();
    void Init(TypeListModel* modelPtr, int index);

public:
    vl::Object& getData() const;

    // Properties
    QString name() const;
    void setName(const QString &newName);
    const std::string& getId() const;

    TypePropListModel *paramListModel();
    Q_PROPERTY (QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY (TypePropListModel* paramListModel READ paramListModel NOTIFY paramListChanged)
    Q_INVOKABLE void newParam();
    Q_INVOKABLE void removeParam(int index);

private:
    int mIndex = -1;
    TypeListModel* mListModel = nullptr;
    TypePropListModel mParamListModel;

signals:
    void nameChanged();
    void paramListChanged();
};

Q_DECLARE_METATYPE(TypeModel)

#endif // OBJECT_H
