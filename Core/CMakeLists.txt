cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

if (LOG_ON)
	add_compile_definitions("LOG_ON")
endif ()

project(DataModelBuilderCore)
option(USE_JSONCONVERTER "Use JSONConverter lib" ON)
option(DMB_TESTS "Include DMBTest target" OFF)

set(DEPS "dependencies")

include_directories(${PROJECT_BINARY_DIR})
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${DEPS}/VL/include")
include_directories("${DEPS}/VL/JSONConverter/include")

if (USE_JSONCONVERTER)
	set (EXTRA_LIBS ${EXTRA_LIBS} JSONConverter)
	add_subdirectory("${DEPS}/VL/JSONConverter")
else()
	set (EXTRA_LIBS ${EXTRA_LIBS} VL)
	add_subdirectory("${DEPS}/VL")
endif () 

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

