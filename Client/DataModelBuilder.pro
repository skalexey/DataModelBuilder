QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/objectlistmodel.cpp \
        src/object.cpp \
        src/objectparameter.cpp \
        src/parameterlistmodel.cpp

RESOURCES += UI/DataModelBuilderUI.qrc \
    src.qrc \
    main.qml

TRANSLATIONS += \
    DataModelBuilder_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = UI/imports

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    engeneering-icon.png

HEADERS += \
    src/objectlistmodel.h \
    src/object.h \
    src/objectparameter.h \
    src/parameterlistmodel.h
