#ifndef OBJECT_H
#define OBJECT_H
#include <QObject>
#include <QString>
#include <memory>
#include "parameterlistmodel.h"

struct Object : public QObject
{
    Q_OBJECT

public:
    Object() = default;
    ~Object() = default;
    Object(const QString& name);

public:
    // Properties
    const QString &name() const
    {
        return mName;
    }

    void setName(const QString &newName)
    {
        if (mName == newName)
            return;
        mName = newName;
        emit nameChanged();
    }

    ObjectParameterListModel *paramListModel();

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(ObjectParameterListModel* paramListModel READ paramListModel NOTIFY paramListChanged)

private:
    QString mName;
    ObjectParameterListModel mParamListModel;

signals:
    void nameChanged();
    void paramListChanged();
};

#endif // OBJECT_H
