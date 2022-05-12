cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

if (LOG_ON)
	add_compile_definitions("LOG_ON")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DataModelBuilderCore)

message("=== DMBCore project CMake build configuration process started ===")

option(USE_JSONCONVERTER "Use JSONConverter lib" ON)
option(DMB_TESTS "Include DMBTest target" OFF)

#message("DMBCore CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Setup dependencies

if ("${DEPS}" STREQUAL "")
	message("WARNING: DEPS is not specified. Take the default value")
	set(DEPS "${PROJECT_SOURCE_DIR}/dependencies")
else()
	message("DEPS: ${DEPS}")
endif()

if (CMAKE_CXX_COMPILER EQUAL "g++")
	set(BUILD_DIR_NAME "Build-g++")
else ()
	set(BUILD_DIR_NAME "Build-cmake")
endif ()

if (USE_JSONCONVERTER)
	set (EXTRA_LIBS ${EXTRA_LIBS} JSONConverter)
	add_subdirectory("${DEPS}/VL/JSONConverter" "${DEPS}/VL/JSONConverter/${BUILD_DIR_NAME}")
else()
	set (EXTRA_LIBS ${EXTRA_LIBS} VL)
	add_subdirectory("${DEPS}/VL" "${DEPS}/VL/${BUILD_DIR_NAME}")
endif () 

set(DMBCORE_INCLUDES
	"${PROJECT_SOURCE_DIR}/include"
	"${JSONCONVERTER_INCLUDES}"
)

set(DMBCORE_INCLUDES ${DMBCORE_INCLUDES} PARENT_SCOPE)

include_directories(${PROJECT_BINARY_DIR})
include_directories("${DMBCORE_INCLUDES}")

file(GLOB SRC "src/*")
file(GLOB HEADERS "include/*")
add_library(DMBCore ${SRC} ${HEADERS})

target_link_libraries(DMBCore ${EXTRA_LIBS})

configure_file(
	"${PROJECT_SOURCE_DIR}/CoreConfig.h.in"
	"${PROJECT_BINARY_DIR}/CoreConfig.h"
)

if (DMB_TESTS)
	add_executable(DMBTest test.cpp)
	target_link_libraries(DMBTest DMBCore)
endif ()

add_custom_command(
    TARGET DMBCore PRE_BUILD
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_dependencies.sh ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
)

