#ifndef OBJECT_H
#define OBJECT_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <memory>
#include "parameterlistmodel.h"

struct Object : public QObject
{
    Q_OBJECT

public:
    explicit Object(QObject* parent = nullptr) : QObject(parent) {}
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

    Q_PROPERTY (QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY (ObjectParameterListModel* paramListModel READ paramListModel NOTIFY paramListChanged)
    Q_INVOKABLE void newParam();
    Q_INVOKABLE void removeParam(int index);
    Q_INVOKABLE ObjectParameter* param(int index);

private:
    QString mName;
    ObjectParameterListModel mParamListModel;

signals:
    void nameChanged();
    void paramListChanged();
};

#endif // OBJECT_H
