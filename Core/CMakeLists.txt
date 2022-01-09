cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

project(DataModelBuilderCore)
option(USE_JSONCONVERTER "Use JSONConverter lib" ON)

include_directories(${PROJECT_BINARY_DIR})
include_directories("${PROJECT_SOURCE_DIR}/VL/VL")

if (USE_JSONCONVERTER)
	set (EXTRA_LIBS ${EXTRA_LIBS} JSONConverter)
	include_directories("${PROJECT_SOURCE_DIR}/VL/JSONConverter")
	add_subdirectory("VL/JSONConverter")
else()
	add_subdirectory("VL/VL")
endif () 

add_library(DMBCore DMBCore.h DMBCore.cpp)
target_link_libraries(DMBCore ${EXTRA_LIBS})

configure_file(
	"${PROJECT_SOURCE_DIR}/CoreConfig.h.in"
	"${PROJECT_BINARY_DIR}/CoreConfig.h"
)

add_executable(Test main.cpp)
target_link_libraries(Test DMBCore)

