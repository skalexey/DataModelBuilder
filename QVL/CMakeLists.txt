# Generated from qdmb.pro.

cmake_minimum_required(VERSION 3.16)
project(qvl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(setup_module)
    if (LOG_ON)
        add_compile_definitions("LOG_ON")
        message("Build QVL with logs")
    else ()
        message("Build QVL without logs")
    endif ()

    find_package(QT NAMES Qt6 COMPONENTS Qml)
    find_package(Qt6 REQUIRED COMPONENTS Core Qml)

    # To build this application you need Qt 6.2.0 or higher
    if (Qt6_VERSION VERSION_LESS 6.2.0)
        message(FATAL_ERROR "You need Qt 6.2.0 or newer to build the application.")
    endif()

    add_dependency_module(${CMAKE_SOURCE_DIR}/../Core DMBCore)

    qt6_add_qml_module(${CMAKE_PROJECT_NAME}
        #OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/imports/QVL"
        VERSION 1.0
        URI "QVL"
        PLUGIN_TARGET ${CMAKE_PROJECT_NAME}
        DESIGNER_SUPPORTED
    )

    include_directories("include")

    file(GLOB sources CONFIGURE_DEPENDS "src/*" "include/*")

    # target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    #     ${sources}
    # )
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
        ${DEPS}/VL/JSONConverter/include
        ${DEPS}/VL/include
        ${DEPS}/Utils/include
        ../Core/include
        include
    )
    
    # Allow debugging QML
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_compile_definitions(qvl PRIVATE QT_QML_DEBUG)
    endif()

    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Qml
    )

    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE QT_QML_DEBUG)
    endif()

    setup_src()
    qt_add_library(${CMAKE_PROJECT_NAME} ${SRC})
    module_message("DEPENDENCY_LIBRARIES: ${DEPENDENCY_LIBRARIES}")
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${DEPENDENCY_LIBRARIES})
    
    add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} PRE_BUILD
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build_dmbcore.sh ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_SOURCE_DIR}"
        VERBATIM
    )

    module_message("BUILD_DIR_NAME: ${BUILD_DIR_NAME}")
    target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Core/${BUILD_DIR_NAME}/$<CONFIG>)
    target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${DEPS}/VL/JSONConverter/$<CONFIG>)
    target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${DEPS}/VL/Build-cmake/$<CONFIG>)
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()