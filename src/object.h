#ifndef OBJECT_H
#define OBJECT_H
#include <QObject>
#include <memory>

class ObjectParameterListModel;
typedef std::unique_ptr<ObjectParameterListModel> ParamListModelPtr;

template <typename StringContainer>
struct Object// : public QObject
{
//    Q_OBJECT

//public:
//    explicit Object(QObject *parent = nullptr);
    Object(const StringContainer& name)
        : name(name)
        , mParamListModel(nullptr)
    {
        mParamListModel = std::make_unique<ObjectParameterListModel>();
    }

    ObjectParameterListModel &paramListModel() const
    {
        return *mParamListModel.get();
    }

    StringContainer name;
    ParamListModelPtr mParamListModel;
};

#endif // OBJECT_H
