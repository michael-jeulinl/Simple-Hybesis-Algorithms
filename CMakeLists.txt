###########################################################################
#
# Project: Simple Hybesis Algorithm
#
# Copyright (c) Michael Jeulin-Lagarrigue.
# All rights reserved.
#
# Author: Michael Jeulin-Lagarrigue.
#
###########################################################################

cmake_minimum_required(VERSION 2.8.2)

#-----------------------------------------------------------------------------
project(SimpleHybesisAlgorithm)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Set a default build type if none was specified
#
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

#-----------------------------------------------------------------------------
# (SimpleHybesisAlgorithm)version number.  
# An even minor number corresponds to releases.
#
set(SHA_VERSION_MAJOR 0)
set(SHA_VERSION_MINOR 1)
set(SHA_VERSION_PATCH 0)
set(SHA_VERSION
    "${SHA_VERSION_MAJOR}.${SHA_VERSION_MINOR}.${SHA_VERSION_PATCH}")

#-----------------------------------------------------------------------------
# Install directories, used for install rules.
#
if(NOT SHA_SOURCE_DIR)
  set(SHA_SOURCE_DIR "src")
endif()
if(NOT SHA_INSTALL_DOC_DIR)
  set(SHA_INSTALL_DOC_DIR "doc")
endif()

#-----------------------------------------------------------------------------
# Testing Options
#
option(BUILD_TESTING "Compile tests on the project sources" ON)
if(BUILD_TESTING)
  include("CMake/GTest.cmake")
endif()

#-----------------------------------------------------------------------------
# Set coverage Flags
#
if(WITH_COVERAGE)
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(coverage_flags "-g -fprofile-arcs -ftest-coverage -O0 -DNDEBUG")
    set(COVERAGE_CXX_FLAGS ${coverage_flags})
    set(COVERAGE_C_FLAGS ${coverage_flags})
  endif()
endif()

#-----------------------------------------------------------------------------
# Additional CXX/C Flags
#
set(ADDITIONAL_C_FLAGS "" CACHE STRING "Additional C Flags")
mark_as_advanced(ADDITIONAL_C_FLAGS)
set(ADDITIONAL_CXX_FLAGS "" CACHE STRING "Additional CXX Flags")
mark_as_advanced(ADDITIONAL_CXX_FLAGS)

#-----------------------------------------------------------------------------
# SHA C/CXX Flags
#
set(SHA_C_FLAGS 
  "${CMAKE_C_FLAGS_INIT} ${COVERAGE_C_FLAGS} ${ADDITIONAL_C_FLAGS}")
set(SHA_CXX_FLAGS 
  "${CMAKE_CXX_FLAGS_INIT} ${VISIBILITY_CXX_FLAGS} ${COVERAGE_CXX_FLAGS} ${ADDITIONAL_CXX_FLAGS}")

if(CMAKE_COMPILER_IS_GNUCXX)
  set(cflags "-Wall -Wextra -Wpointer-arith -Winvalid-pch -Wcast-align -Wwrite-strings -D_FORTIFY_SOURCE=2")

  if(MINGW)
    # suppress warnings about auto imported symbols
    set(SHA_CXX_FLAGS "-Wl,--enable-auto-import ${SHA_CXX_FLAGS}")
  endif()

  set(SHA_C_FLAGS "${cflags} ${SHA_C_FLAGS}")
  set(SHA_CXX_FLAGS "${cflags} 
      -Woverloaded-virtual -Wold-style-cast 
	  -Wstrict-null-sentinel -Wsign-promo ${SHA_CXX_FLAGS}")
endif()

if(MSVC)
  set(msvc_suppressed_warnings
    "/wd4290" # C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
	)
  
  set(SHA_CXX_FLAGS "${SHA_CXX_FLAGS} ${msvc_suppressed_warnings}")
endif()

add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE 
                -D_CRT_SECURE_NO_WARNINGS 
                -D_CRT_SECURE_NO_DEPRECATE)

#-----------------------------------------------------------------------------
# Set C/CXX Flags
#
set(CMAKE_CXX_FLAGS ${SHA_CXX_FLAGS} CACHE STRING "CMake C Flags" FORCE)
set(CMAKE_C_FLAGS ${SHA_C_FLAGS} CACHE STRING "CMake CXX Flags" FORCE)

#-----------------------------------------------------------------------------
# Add the modules
#
add_subdirectory(Modules)
