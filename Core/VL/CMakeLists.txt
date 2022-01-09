cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Uncomment the next line if want to experiment with g++ compliation
#set(CMAKE_CXX_COMPILER "g++")

project(VLTests)
option(JSON "Connect JSON support infrastructure (JSONConverter)" ON)

include_directories(${PROJECT_BINARY_DIR})
if (JSON)
	include_directories(${PROJECT_SOURCE_DIR}/JSONConverter)
	add_subdirectory("JSONConverter")
	set (EXTRA_LIBS ${EXTRA_LIBS} JSONConverter)
endif ()

include_directories("VL")
#add_subdirectory("VL")
add_executable(Test test.cpp)
target_link_libraries(Test VL)
target_link_libraries(Test ${EXTRA_LIBS})
