# // ------------------------------------------------------------------------------ \\
# 
# This is root CMake-File for project: 'gzip_util'.
# 
# About:
# "gzip_util" - utility to work with gzip.
# 
# License: see "LICENSE" file.
# API: C++ 11 for mobile, C++ 17 for PC (Windows & Linux).
# Platforms: Any with C++ 11 support.
# 
# // ------------------------------------------------------------------------------ \\

# =================================================================================
# CMake Meta-Data
# =================================================================================

# CMake-Version
cmake_minimum_required ( VERSION 3.8 FATAL_ERROR )

# =================================================================================
# PROJECT
# =================================================================================

# Project Name
set ( ROOT_PROJECT_NAME "gzip_util" )

# Project Version
set ( ROOT_PROJECT_VERSION 0.1.0 )

# Configure Project
project ( ${ROOT_PROJECT_NAME} VERSION ${ROOT_PROJECT_VERSION} LANGUAGES C CXX )

# =================================================================================
# CONFIG
# =================================================================================

# DETECT LINUX
if ( NOT ANDROID AND UNIX AND NOT DEFINED LINUX AND NOT APPLE )
		
	# LINUX ON
	set ( LINUX ON )
	
endif ( NOT ANDROID AND UNIX AND NOT DEFINED LINUX AND NOT APPLE )

# PLATFORM
if ( ANDROID ) # ANDROID

	# Set Directory Prefix
	set ( PLATFORM_DIR "android" )
	
elseif ( WIN32 ) # WINDOWS

	# Set Directory Prefix
	set ( PLATFORM_DIR "win32" )

elseif ( LINUX ) # LINUX
	
	# Set Directory Prefix
	set ( PLATFORM_DIR "linux" )

elseif ( APPLE ) # APPLE

	# Set Directory Prefix
	set ( PLATFORM_DIR "apple" )

else ( ANDROID ) # ?
	
	# ERROR
	message ( SEND_ERROR "${ROOT_PROJECT_NAME} - unknown platform ! Configuration required." )
	
	# Set Directory Prefix
	set ( PLATFORM_DIR "unix" )
	
endif ( ANDROID )

# Build-Type Sub-Dir
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )

	# Add Definition
	add_definitions ( -DDEBUG=1 )
	
	# Set Build-Type Dir
	set ( BUILD_TYPE_DIR "debug" )
	
else ( CMAKE_BUILD_TYPE STREQUAL "Debug" )

	# Set Build-Type Dir
	set ( BUILD_TYPE_DIR "release" )
	
endif ( CMAKE_BUILD_TYPE STREQUAL "Debug" )

# Compiler flags
if ( MSVC ) # MSVC
	# W3 - Display more warning
	# std=c++17 - C++ version
	set ( CMAKE_CXX_FLAGS "/W3 /std:c++17 /Zc:__cplusplus /EHsc" )
elseif ( ANDROID ) # Android NDK
elseif ( UNIX OR CMAKE_COMPILER_IS_GNUCXX ) # Clang || gcc (Linux, Mac OS or Win32 with MingW)

	# Flags
	set ( CMAKE_CXX_FLAGS "-Wall -std=c++17 -EHsc" )

	# Apple
	if ( APPLE ) # Clang / Mac OS only
		set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -mmacosx-version-min=10.7" )
	endif ( APPLE ) # Clang / Mac OS only
endif ( MSVC ) # MSVC

# =================================================================================
# CONSTANTS
# =================================================================================

# Set Sources Dir
set ( SOURCES_DIR "${CMAKE_SOURCE_DIR}/src/cpp" )

# Set Project Bin-Output Dir
set ( ROOT_PROJECT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${BUILD_TYPE_DIR}/${PLATFORM_DIR}" )

# Libs Dir
set ( ROOT_PROJECT_LIBS_DIR "${CMAKE_SOURCE_DIR}/libs/lib" )

# Include Dir
set ( ROOT_PROJECT_LIBS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/include" )

# =================================================================================
# LIBRARIES
# =================================================================================

# =============== zlib ====================

# PLATFORM
if ( WIN32 ) # WINDOWS
	# Z-LIB LIB LOCATION
	set ( ZLIB_LIB_LOCATION "${ROOT_PROJECT_LIBS_DIR}/zlib/release/win32/x86_32/zlib.lib" )

	# Z-LIB INCLUDES
	set ( ZLIB_INCLUDE_DIR "${ROOT_PROJECT_LIBS_INCLUDE_DIR}/win32/zlib" )
elseif ( LINUX )
	# ERROR
	message ( FATAL_ERROR "${ROOT_PROJECT_NAME} - missing Linux configuration ! Configuration required." )
else ( WIN32 )
	# ERROR
	message ( FATAL_ERROR "${ROOT_PROJECT_NAME} - unknown platform ! Configuration required." )
endif ( WIN32 ) # WINDOWS

# Check Z-LIB LOCATION
if ( NOT EXISTS ${ZLIB_LIB_LOCATION} )
	# ERROR
	message ( FATAL_ERROR "${ROOT_PROJECT_NAME} - zlib not found at ${ZLIB_LIB_LOCATION}" )
else ( NOT EXISTS ${ZLIB_LIB_LOCATION} )
	# INFO
	message ( STATUS "${ROOT_PROJECT_NAME} - zlib found at ${ZLIB_LIB_LOCATION}" )
endif ( NOT EXISTS ${ZLIB_LIB_LOCATION} )

# Check Z-LIB INCLUDE DIR
if ( NOT EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h" )
	# ERROR
	message ( FATAL_ERROR "${ROOT_PROJECT_NAME} - zlib includes not found at ${ZLIB_INCLUDE_DIR}" )
else ( NOT EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h" )
	# INFO
	message ( STATUS "${ROOT_PROJECT_NAME} - zlib includes found at ${ZLIB_INCLUDE_DIR}" )
endif ( NOT EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h" )

# Add STATIC zlib Library
add_library( zlib STATIC IMPORTED )

# Set zlib Library-Object Properties
set_target_properties( zlib PROPERTIES
IMPORTED_LOCATION "${ZLIB_LIB_LOCATION}" 
INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIR}" )

# INFO
message ( STATUS "${ROOT_PROJECT_NAME} - zlib Imported as STATIC Library" )

# =================================================================================
# HEADERS
# =================================================================================

set ( ROOT_PROJECT_HEADERS "${SOURCES_DIR}/main.hpp"
"${SOURCES_DIR}/zip/ZStream.hpp" )

# =================================================================================
# SOURCES
# =================================================================================

set ( ROOT_PROJECT_SOURCES "${SOURCES_DIR}/main.cpp"
"${SOURCES_DIR}/zip/ZStream.cpp" )

# =================================================================================
# PRECOMPILED HEADERS
# =================================================================================

# Headers
set ( ROOT_PROJECT_HEADERS "${SOURCES_DIR}/pch_cxx.hpp" "${ROOT_PROJECT_HEADERS}" )

# Sources
set ( ROOT_PROJECT_SOURCES "${SOURCES_DIR}/pch_cxx.cpp" "${ROOT_PROJECT_SOURCES}" )

# =================================================================================
# EXECUTABLE RESOURCES
# =================================================================================

# PLATFORM
if (  WIN32 ) # WINDOWS
	set ( ROOT_PROJECT_RESOURCES "${CMAKE_SOURCE_DIR}/res/win32_resources.rc" )
else ( WIN32 )
	message ( FATAL_ERROR "${ROOT_PROJECT_NAME} - executable resources configuration required !" )
endif ( WIN32 ) # WINDOWS

# =================================================================================
# BUILD EXECUTABLE
# =================================================================================

# PLATFORM
if ( WIN32 ) # WINDOWS
	# Create Executable Object
	add_executable ( gzip_util ${ROOT_PROJECT_SOURCES} ${ROOT_PROJECT_HEADERS} ${ROOT_PROJECT_RESOURCES} )
	
	# Configure Executable Object
	set_target_properties ( gzip_util PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
	CXX_EXTENSIONS FALSE
	OUTPUT_NAME "${ROOT_PROJECT_NAME}_v${ROOT_PROJECT_VERSION}"
	RUNTIME_OUTPUT_DIRECTORY ${ROOT_PROJECT_OUTPUT_DIR} )
	
	# Link
	target_link_libraries ( gzip_util zlib )

	# Request features
	target_compile_features ( gzip_util PRIVATE cxx_std_17 )
else ( WIN32 )
	message ( FATAL_ERROR "${ROOT_PROJECT_NAME} - executable object configuration required !" )
endif ( WIN32 ) # WINDOWS