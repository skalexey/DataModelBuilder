cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

project(JSONConverter)
option(BUILD_TESTS "Add Test target" OFF)

include_directories(${PROJECT_BINARY_DIR})
include_directories("${PROJECT_SOURCE_DIR}/VL")
include_directories("${PROJECT_SOURCE_DIR}/include"})

add_subdirectory("VL")

set (EXTRA_LIBS ${EXTRA_LIBS} VL)

add_library(JSONConverter JSONDefs.h JSONConverter.h JSONConverter.cpp JSONWriter.h JSONWriter.cpp JSONLoader.h JSONLoader.cpp)
target_link_libraries (JSONConverter VL)

configure_file(
	"${PROJECT_SOURCE_DIR}/JSONConverterConfig.h.in"
	"${PROJECT_BINARY_DIR}/JSONConverterConfig.h"
)

if (BUILD_TESTS)
	add_executable(Test test.cpp)
	target_link_libraries(Test JSONConverter)
endif ()
