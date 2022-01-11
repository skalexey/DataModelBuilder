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
    main.qml

TRANSLATIONS += \
    DataModelBuilder_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations
CONFIG += c+17

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

# DMBCore.lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Core/Build/Release/ -lDMBCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Core/Build/Debug/ -lDMBCore
else:unix: LIBS += -L$$PWD/../Core/Build-g++/ -lDMBCore

INCLUDEPATH += $$PWD/../Core/include
DEPENDPATH += $$PWD/../Core/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build-g++/libDMBCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build-g++/libDMBCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build/Release/DMBCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build/Debug/DMBCore.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Core/Build-g++/libDMBCore.a

# JSONConverter.lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Core/Build/dependencies/VL/JSONConverter/Release/ -lJSONConverter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Core/Build/dependencies/VL/JSONConverter/Debug/ -lJSONConverter
else:unix: LIBS += -L$$PWD/../Core/Build-g++/dependencies/VL/JSONConverter/ -lJSONConverter

INCLUDEPATH += $$PWD/../Core/dependencies/VL/JSONConverter/include
DEPENDPATH += $$PWD/../Core/dependencies/VL/JSONConverter/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build-g++/dependencies/VL/JSONConverter/libJSONConverter.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build-g++/dependencies/VL/JSONConverter/libJSONConverter.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build/dependencies/VL/JSONConverter/Release/JSONConverter.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/Build/dependencies/VL/JSONConverter/Debug/JSONConverter.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Core/Build-g++/dependencies/VL/JSONConverter/libJSONConverter.a

# VL.lib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Core/dependencies/VL/Build/Release/ -lVL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Core/dependencies/VL/Build/Debug/ -lVL
else:unix: LIBS += -L$$PWD/../Core/dependencies/VL/Build/ -lVL

INCLUDEPATH += $$PWD/../Core/dependencies/VL/include
DEPENDPATH += $$PWD/../Core/dependencies/VL/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build-g++/libVL.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build-g++/libVL.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build/Release/VL.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build/Debug/VL.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Core/dependencies/VL/Build-g++/libVL.a
