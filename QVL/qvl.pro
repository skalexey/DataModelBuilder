TEMPLATE = lib
CONFIG += plugin qmltypes
#CONFIG += static
CONFIG += c+17
QT += qml
TARGET = qvl

QML_IMPORT_NAME = QVL
QML_IMPORT_MAJOR_VERSION = 1

DESTDIR = imports/$$QML_IMPORT_NAME
QMLTYPES_FILENAME = $$DESTDIR/plugin.qmltypes

#QMAKE_MOC_OPTIONS += -Muri=qvl

INCLUDEPATH += include
#INCLUDEPATH += src

SOURCES += \
    plugin.cpp \
    src/DMBModel.cpp \
    src/DMBUtils.cpp \
    src/ObjectProperty.cpp \
    src/Utils.cpp \
    src/VLCollectionModel.cpp \
    src/VLListModel.cpp \
    src/VLListModelInterface.cpp \
    src/VLListVarModel.cpp \
    src/VLObjectVarModel.cpp \
    src/VLVarModel.cpp \
    src/VarModelFactory.cpp

HEADERS += \
    include/DMBModel.h \
    include/DMBUtils.h \
    include/ModelsFwd.h \
    include/ObjectProperty.h \
    include/Utils.h \
    include/VLCollectionModel.h \
    include/VLListModel.h \
    include/VLListModelInterface.h \
    include/VLListVarModel.h \
    include/VLObjectVarModel.h \
    include/VLVarModel.h \
    include/VarModelFactory.h

PLUGINFILES += imports/$$QML_IMPORT_NAME/qmldir
RESOURCES += imports/$$QML_IMPORT_NAME/qmldir

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Core/Build/Release/ -lDMBCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Core/Build/Debug/ -lDMBCore

INCLUDEPATH += $$PWD/../Core/include
DEPENDPATH += $$PWD/../Core/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build-g++/libDMBCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build-g++/libDMBCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build/Release/DMBCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build/Debug/DMBCore.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Core/Build/dependencies/VL/JSONConverter/Release/ -lJSONConverter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Core/Build/dependencies/VL/JSONConverter/Debug/ -lJSONConverter

INCLUDEPATH += $$PWD/../Core/dependencies/VL/JSONConverter/include
DEPENDPATH += $$PWD/../Core/dependencies/VL/JSONConverter/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build-g++/dependencies/VL/JSONConverter/libJSONConverter.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build-g++/dependencies/VL/JSONConverter/libJSONConverter.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build/dependencies/VL/JSONConverter/Release/JSONConverter.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build/dependencies/VL/JSONConverter/Debug/JSONConverter.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Core/dependencies/VL/Build/Release/ -lVL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Core/dependencies/VL/Build/Debug/ -lVL

INCLUDEPATH += $$PWD/../Core/dependencies/VL/include
DEPENDPATH += $$PWD/../Core/dependencies/VL/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build-g++/libVL.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build-g++/libVL.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build/Release/VL.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build/Debug/VL.lib

message("Desttir: $$DESTDIR")
message("OUT_PWD: $$OUT_PWD")

#target.path =  $$DESTDIR

pluginfiles_copy.files = $$PLUGINFILES
pluginfiles_copy.path = $$OUT_PWD/imports/$$QML_IMPORT_NAME

#pluginfiles_install.files = $$PLUGINFILES $$OUT_PWD/$$DESTDIR/plugin.qmltypes
#pluginfiles_install.path = $$DESTDIR


#INSTALLS += target pluginfiles_install
COPIES += pluginfiles_copy

#OTHER_FILES += $$PLUGINFILES

#CONFIG += install_ok  # Do not cargo-cult this!
