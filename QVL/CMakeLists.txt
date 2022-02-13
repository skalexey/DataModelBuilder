# Generated from qdmb.pro.

message("CMake version: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}")

cmake_minimum_required(VERSION 3.16)
project(qvl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (LOG_ON)
	add_compile_definitions("LOG_ON")
endif ()

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Qml)

qt6_add_qml_module(qvl
    #OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/imports/QVL"
    VERSION 1.0
    URI "QVL"
    PLUGIN_TARGET qvl
    DESIGNER_SUPPORTED
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

target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-cmake/$<CONFIG>)
target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/Build-cmake/dependencies/VL/JSONConverter/$<CONFIG>)
target_link_directories(qvl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/dependencies/VL/Build-cmake/$<CONFIG>)
