# Generated from DataModelBuilder.pro.

cmake_minimum_required(VERSION 3.16)
project(DataModelBuilder LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Quick)


file(GLOB sources CONFIGURE_DEPENDS "src/*")

qt_add_executable(DataModelBuilder WIN32 MACOSX_BUNDLE
    main.cpp
    ${sources}
    UI/DataModelBuilderUI.qrc
)
target_link_libraries(DataModelBuilder PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
)

#message(STATUS "QML_IMPORT_PATH content: ${QML_IMPORT_PATH}")
#list(APPEND QML_DIRS "UI")
#set(QML_IMPORT_PATH "${QML_DIRS}" CACHE STRING "Qt Creator extra qml import paths")

# Make Qt Creator aware of where the QML modules live
#set (_QML_IMPORT_PATHS "UI/imports")
## Add new module paths here.
#list (APPEND _QML_IMPORT_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/UI)

set (
    QML_IMPORT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/imports
    CACHE
    STRING
    "Path used to locate CMake modules by Qt Creator"
    FORCE
)

message(STATUS "QML_IMPORT_PATH content: ${QML_IMPORT_PATH}")

set(qmake_immediate_resource_files
    "main.qml"
)

qt6_add_resources(DataModelBuilder "qmake_immediate"
    PREFIX
        "/"
    FILES
        ${qmake_immediate_resource_files}
)

install(TARGETS DataModelBuilder
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
