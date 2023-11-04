cmake_minimum_required(VERSION 3.10)
project(ExampleDistccMacos)

if (APPLE)
	find_program(DISTCC_PROGRAM "distcc")
	if (DISTCC_PROGRAM)
		message(STATUS "Distcc found: ${DISTCC_PROGRAM}")
		set(DISTCC_CC "${CMAKE_BINARY_DIR}/distcc_cc.sh")
		set(DISTCC_CXX "${CMAKE_BINARY_DIR}/distcc_cxx.sh")
		# For testing purposes, DISTCC_FALLBACK is defined as 0 to force all
		# compilations to be done on remote machines.
		file(WRITE ${DISTCC_CC}
			"#!/bin/sh\n"
			"export DISTCC_FALLBACK=0\n"
			"${DISTCC_PROGRAM} ${CMAKE_C_COMPILER} \"$@\""
		)
		file(WRITE ${DISTCC_CXX}
			"#!/bin/sh\n"
			"export DISTCC_FALLBACK=0\n"
			"${DISTCC_PROGRAM} ${CMAKE_CXX_COMPILER} \"$@\""
		)
		execute_process(COMMAND chmod +x ${DISTCC_CC})
		execute_process(COMMAND chmod +x ${DISTCC_CXX})
		if (CMAKE_GENERATOR STREQUAL "Xcode")
			set(CMAKE_XCODE_ATTRIBUTE_CC ${DISTCC_CC})
			set(CMAKE_XCODE_ATTRIBUTE_CXX ${DISTCC_CXX})
			# Disable the indexing feature in Xcode's build settings to prevent
			# issues with distcc. The `-index-store-path` compiler flag causes
			# problems with distcc's distributed compilation process.
			set(CMAKE_XCODE_ATTRIBUTE_COMPILER_INDEX_STORE_ENABLE NO)
		elseif (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
			set(CMAKE_C_COMPILER_LAUNCHER ${DISTCC_CC})
			set(CMAKE_CXX_COMPILER_LAUNCHER ${DISTCC_CXX})
		else()
			message(WARNING "Only 'Xcode' and 'Unix Makefiles' generators are supported.")
 		endif()
	else()
		message(WARNING "Distcc not found.")
	endif()
endif()

file(GLOB SOURCES
	"source/*.cpp"
	"source/*.h"
)

add_executable(
	${CMAKE_PROJECT_NAME}
	${SOURCES}
)
