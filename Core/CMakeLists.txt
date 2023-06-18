cmake_minimum_required(VERSION 3.21)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

project(DMBCore C CXX)

if (LOG_ON)
	add_compile_definitions("LOG_ON")
endif ()

if(MSVC)
	# fix __cplusplus macro for MSVC
	add_compile_options(
		"/Zc:__cplusplus"
		# "/std:c++latest"
	)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("=== DMBCore project CMake build configuration process started ===")

option(USE_JSONCONVERTER "Use JSONConverter lib" ON)
option(DMB_TESTS "Include DMBTest target" OFF)

#message("DMBCore CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# set deps variables
set(DMBCore_DEPS "$ENV{DMBCore_deps}")
set(DMBCore_DEPS "${DMBCore_DEPS}" PARENT_SCOPE)
if ("${DMBCore_DEPS}" STREQUAL "")
	file(TO_CMAKE_PATH "$ENV{build_deps}" DEPS)
endif()
if ("${DEPS}" STREQUAL "")
	set(DEPS ${DMBCore_DEPS})
endif()
if ("${DEPS}" STREQUAL "")
	message(SEND_ERROR "DMBCore: ERROR: DEPS is not specified. Take the default value")
	set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
else()
	message("DMBCore: DEPS: ${DEPS}")
endif()
set(DEPS ${DEPS} PARENT_SCOPE)

# build directory name
if (CMAKE_CXX_COMPILER EQUAL "g++")
	set(BUILD_DIR_NAME "Build-g++")
else ()
	set(BUILD_DIR_NAME "Build-cmake")
endif ()

# VL and JSONConverter dependencies
if (USE_JSONCONVERTER)
	set (EXTRA_LIBS ${EXTRA_LIBS} JSONConverter)
	add_subdirectory("${DEPS}/VL/JSONConverter" "${DEPS}/VL/JSONConverter/${BUILD_DIR_NAME}")
	set(DMBCore_INCLUDES ${DMBCore_INCLUDES} "${JSONCONVERTER_INCLUDES}")
else()
	set (EXTRA_LIBS ${EXTRA_LIBS} VL)
	add_subdirectory("${DEPS}/VL" "${DEPS}/VL/${BUILD_DIR_NAME}")
	set(DMBCore_INCLUDES ${DMBCore_INCLUDES} "${VL_INCLUDES}")
endif () 

set(DMBCore_INCLUDES ${DMBCore_INCLUDES} "${PROJECT_SOURCE_DIR}/include")
set(DMBCore_INCLUDES ${DMBCore_INCLUDES} PARENT_SCOPE)

include_directories(${PROJECT_BINARY_DIR})
include_directories("${DMBCore_INCLUDES}")

file(GLOB SRC "src/*")
file(GLOB HEADERS "include/*")
add_library(DMBCore STATIC ${SRC} ${HEADERS})

target_link_libraries(DMBCore ${EXTRA_LIBS})

configure_file(
	"${PROJECT_SOURCE_DIR}/CoreConfig.h.in"
	"${PROJECT_BINARY_DIR}/CoreConfig.h"
)

if (DMB_TESTS)
	message("DMB_TESTS is enabled")
	add_executable(DMBTest test.cpp)
	target_link_libraries(DMBTest DMBCore)
else()
	message("DMB_TESTS is disabled")
endif ()

# add_custom_command(
#     TARGET DMBCore PRE_BUILD
#     COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_dependencies.sh ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_SOURCE_DIR}"
#     VERBATIM
# )

