#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include <QObject>
#include <QVariant>
#include <QVector>

enum ObjectParamType {
    ParamTypeString,
    ParamTypeImage,
    ParamTypeFlag,
    ParamTypeObject,
    ParamTypeListOfObjects
};

struct ObjectParam
{
    ObjectParam() = default;
    ~ObjectParam() = default;
    ObjectParam(const ObjectParam& right) = default;
    ObjectParam& operator=(const ObjectParam& right) = default;
    bool operator == (const ObjectParam& right) const;
    QString name;
    ObjectParamType type;
};

struct ObjectListItem
{
    bool operator == (const ObjectListItem& right) const;
    QString name;
    QVector<ObjectParam> params;
    QVariantList paramsQVariantList() const;
};

class ObjectList : public QObject
{
    Q_OBJECT
public:
    explicit ObjectList(QObject *parent = nullptr);
    bool setItemAt(int index, const ObjectListItem& item);
    QVector<ObjectListItem> items() const;

public slots:
    void appendItem();

signals:
    void preItemAppended();
    void postItemAppended();
    void preItemRemoved(int index);
    void postItemRemoved();

private:
    QVector<ObjectListItem> mItems;
};

#endif // OBJECTLIST_H
