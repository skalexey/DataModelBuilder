QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include

SOURCES += \
        main.cpp \
        src/AppObject.cpp

HEADERS += \
    include/AppObject.h

RESOURCES +=  \
    UI/DataModelBuilderUI.qrc \
    main.qml

TRANSLATIONS += \
    DataModelBuilder_en_US.ts

CONFIG += lrelease
CONFIG += embed_translations
CONFIG += c+17

#QMAKE_MOC_OPTIONS += -Muri=QDMBPlugin

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = imports UI/imports


# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




#win32: LIBS += -L$$PWD/imports/DMBModel/ -lqdmbplugin

INCLUDEPATH += $$PWD/imports/QVL
DEPENDPATH += $$PWD/imports/QVL

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/imports/DMBModel/qdmbplugin.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/imports/DMBModel/libqdmbplugin.a
