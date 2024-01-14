cmake_minimum_required (VERSION 3.15)

project(DMBCore C CXX)

macro(setup_module)
	module_message("Setting up module: ${MODULE_NAME}")
	module_message("Parent dir: ${PARENT_DIR}")
	
	option(USE_JSONCONVERTER "Use JSONConverter lib" ON)
	option(DMB_TESTS "Include DMBTest target" OFF)
	option(DMBCORE_LOG_VERBOSE "Enable verbose logging" ON)
	
	# VL and JSONConverter dependencies
	if (USE_JSONCONVERTER)
		set (EXTRA_LIBS ${EXTRA_LIBS} JSONConverter)
		add_dependency_module("${DEPS}/VL/JSONConverter" JSONConverter)
	else()
		set (EXTRA_LIBS ${EXTRA_LIBS} VL)
		add_dependency_module("${DEPS}/VL" VL)
	endif () 

	configure_file(
		"${PROJECT_SOURCE_DIR}/CoreConfig.h.in"
		"${PROJECT_BINARY_DIR}/CoreConfig.h"
	)

	if (DMB_TESTS)
		module_message("DMB_TESTS is enabled")
		add_executable(DMBTest test.cpp)
		target_link_libraries(DMBTest DMBCore)
	else()
		module_message("DMB_TESTS is disabled")
	endif ()

	# Add headers to sources for IDE
	add_headers_to_sources()
	group_headers_and_sources()

	module_add_library()
	# add_custom_command(
	#     TARGET DMBCore PRE_BUILD
	#     COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_dependencies.sh ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_SOURCE_DIR}"
	#     VERBATIM
	# )
endmacro()

include(cmake_modules/modules.cmake)
modules_include_all(cmake_modules)
setup_module_project()
