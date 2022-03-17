# Generated using pro2cmake from DataModelBuilder.pro and then edited

macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    message(STATUS "Copy from ${srcDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    COPYONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)

cmake_minimum_required(VERSION 3.16)
project(DataModelBuilder LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Quick)

include_directories("include")

file(GLOB sources CONFIGURE_DEPENDS "src/*" "include/*")

qt_add_executable(DataModelBuilder WIN32 MACOSX_BUNDLE
	main.cpp
	${sources}
	UI/DataModelBuilderUI.qrc
)


if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(DataModelBuilder PRIVATE QT_QML_DEBUG)
endif()

target_link_libraries(DataModelBuilder PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
)

set (
    QML_IMPORT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/imports ${CMAKE_CURRENT_SOURCE_DIR}/UI/imports
    CACHE
    STRING
    "Path used to locate CMake modules by Qt Creator"
    FORCE
)

set(qmake_immediate_resource_files
    "main.qml"
)

qt6_add_resources(DataModelBuilder "qmake_immediate"
    PREFIX
        "/"
    FILES
        ${qmake_immediate_resource_files}
)

add_custom_command(
    TARGET DataModelBuilder PRE_BUILD
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/build_qvl.sh ${CMAKE_CFG_INTDIR} "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
)

add_custom_command(
    TARGET DataModelBuilder POST_BUILD
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/deliver_qvl.sh ${CMAKE_CFG_INTDIR} "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
)


