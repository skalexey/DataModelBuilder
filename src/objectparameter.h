#ifndef OBJECTPARAMETER_H
#define OBJECTPARAMETER_H
#include <QObject>

enum ObjectParamType {
    ParamTypeString,
    ParamTypeImage,
    ParamTypeFlag,
    ParamTypeObject,
    ParamTypeListOfObjects
};

template <typename StringContainer>
struct ObjectParameter
{
    StringContainer name;
    ObjectParamType type;
};

#endif // OBJECTPARAMETER_H
