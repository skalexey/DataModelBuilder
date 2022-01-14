#ifndef PARAMETERLISTMODEL_H
#define PARAMETERLISTMODEL_H

#include <QAbstractListModel>
#include <QVariantList>
#include <vector>
#include <functional>
#include <memory>

namespace vl
{
    class AbstractVar;
    typedef AbstractVar Var;
    typedef std::shared_ptr<Var> VarPtr;
}

class TypeModel;

class TypePropListModel : public QAbstractListModel
{
    typedef std::vector<std::string> PropIdList;

    Q_OBJECT
    enum Role {
        RoleName,
        RoleType
    };

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

    static QVariantList typeList;
    static Type GetVLType(const vl::Var& var);
    static vl::VarPtr MakeVar(Type type);

private:
    static QVariantList composeTypeList();

public:
    explicit TypePropListModel(QObject *parent = nullptr);
    TypePropListModel(TypeModel* objectPtr);
    void Init(TypeModel* objectPtr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int,QByteArray> roleNames() const override;
    // Custom:
    void resetList(const std::function<void()>& doWithList = nullptr);
    bool addParam(const std::string& name, TypePropListModel::Type type);
    void removeParam(int index);
   // ObjectParameter* getParam(int index);

signals:
    void nameAlreadyTaken();

private:
    PropIdList mList;
    TypeModel* mObject = nullptr;
};

#endif // PARAMETERLISTMODEL_H
