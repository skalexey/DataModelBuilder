# Generated from qdmb.pro.

cmake_minimum_required(VERSION 3.16)
project(qvl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Qml)

qt6_add_qml_module(qvl
    #OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/imports/QVL"
    VERSION 1.0
    URI "QVL"
    PLUGIN_TARGET qvl
)

include_directories("include")

file(GLOB sources CONFIGURE_DEPENDS "src/*" "include/*")

target_sources(qvl PRIVATE
    ${sources}
)
target_include_directories(qvl PUBLIC
    ../Core/dependencies/VL/JSONConverter/include
    ../Core/dependencies/VL/include
    ../Core/include
    include
)

target_link_libraries(qvl PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    DMBCore
    JSONConverter
    VL
)

#target_compile_definitions(qvl PRIVATE QT_QML_DEBUG)

message("CMake build type: ${CMAKE_BUILD_TYPE}")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("=== Set ${CMAKE_BUILD_TYPE} link directories")
    target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-cmake-Debug/Debug)
    target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-cmake-Debug/dependencies/VL/JSONConverter/Debug)
    target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build-cmake-Debug/Debug)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("=== Set ${CMAKE_BUILD_TYPE} link directories")
    target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-cmake-Release/Release/)
    target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-cmake-Release/dependencies/VL/JSONConverter/Release)
    target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build-cmake-Release/Release)
endif()