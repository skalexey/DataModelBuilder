# Generated from DataModelBuilder.pro.

cmake_minimum_required(VERSION 3.16)
project(DataModelBuilder LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIRn)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

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

target_compile_definitions(DataModelBuilder PRIVATE QT_QML_DEBUG)

target_include_directories(DataModelBuilder PUBLIC
    ../Core/dependencies/VL/JSONConverter/include
    ../Core/dependencies/VL/include
    ../Core/include
)

#target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Core/Build-g++)
#target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-g++/dependencies/VL/JSONConverter)
#target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../???)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("=== Set ${CMAKE_BUILD_TYPE} link directories")
    target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/Debug/)
    target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/dependencies/VL/JSONConverter/Debug)
    target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build/Debug)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("=== Set ${CMAKE_BUILD_TYPE} link directories")
    target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/Release/)
    target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/dependencies/VL/JSONConverter/Release)
    target_link_directories(DataModelBuilder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build/Release)
endif()



target_link_libraries(DataModelBuilder PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    DMBCore
    JSONConverter
    VL
)

set (
    QML_IMPORT_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/imports
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

message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

#link_directories(
#    ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/Debug/
#    ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/dependencies/VL/JSONConverter/Debug/
#    ${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build/Debug/
#)

#target_link_libraries(DataModelBuilder DMBCore)
#target_link_libraries(DataModelBuilder JSONConverter)
#target_link_libraries(DataModelBuilder VL)

#if(WIN32 AND CONFIG(release,debug OR release))
#    target_link_libraries(DataModelBuilder PUBLIC
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/Release/
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/dependencies/VL/JSONConverter/Release/
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build/Release/
#        DMBCore
#        JSONConverter
#        VL
#    )
#endif()

#if((NOT (WIN32 AND CONFIG(release,debug OR release))) AND (WIN32 AND CONFIG(debug,debug OR release)))
#    target_link_libraries(DataModelBuilder PUBLIC
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/Debug/
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build/dependencies/VL/JSONConverter/Debug/
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build/Debug/
#        DMBCore
#        JSONConverter
#        VL
#    )
#endif()

#if(((NOT (WIN32 AND CONFIG(release,debug OR release))) AND (NOT (WIN32 AND CONFIG(debug,debug OR release)))) AND (UNIX))
#    target_link_libraries(DataModelBuilder PUBLIC
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-g++/
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-g++/dependencies/VL/JSONConverter/
        # Remove: L${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build/
#        DMBCore
#        JSONConverter
#        VL
#    )
#endif()

install(TARGETS DataModelBuilder
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
