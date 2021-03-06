#############################################################################################################
#
# SHA - Simple Hybesis Algorithm
#
# Copyright (c) Michael Jeulin-Lagarrigue
#
#  Licensed under the MIT License, you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms/blob/master/LICENSE
#
# Unless required by applicable law or agreed to in writing, software distributed under the License is
# distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and limitations under the License.
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
#############################################################################################################

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
# Version number.
# An even minor number corresponds to releases.
#
set(SHA_VERSION_MAJOR 0)
set(SHA_VERSION_MINOR 2)
set(SHA_VERSION_PATCH 0)
set(SHA_VERSION
    "${SHA_VERSION_MAJOR}.${SHA_VERSION_MINOR}.${SHA_VERSION_PATCH}")

#-----------------------------------------------------------------------------
# Information.
# Variables containing project information.
#
set(SHA_PROJECT_TITLE "Simple-Hybesis-Algorithms")

#-----------------------------------------------------------------------------
# Install directories, used for install rules.
#
if(NOT SHA_SOURCE_DIR)
  set(SHA_SOURCE_DIR "Modules")
endif()
if(NOT SHA_INSTALL_DOC_DIR)
  set(SHA_INSTALL_DOC_DIR "Doc")
endif()

#-----------------------------------------------------------------------------
# Documentation / Doxygen Options
#
option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # Set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doc/Doxyfile.txt.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # Build the docs together with the application
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation.")
endif (DOXYGEN_FOUND)

#-----------------------------------------------------------------------------
# Testing Options
#
option(BUILD_TESTING "Compile tests on the project sources" ON)
if(BUILD_TESTING)
  include("CMake/GTest.cmake")
  ENABLE_TESTING()
  INCLUDE(CTest)
endif()

#-----------------------------------------------------------------------------
# Set coverage Flags
#
option(WITH_COVERAGE "Run tests with coverage on the project sources" ON)
if(WITH_COVERAGE)
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(coverage_flags "-g -fprofile-arcs -ftest-coverage -O0 -DNDEBUG")
    set(COVERAGE_CXX_FLAGS ${coverage_flags})
    set(COVERAGE_C_FLAGS ${coverage_flags})
  endif()

  # CTest coverage configuration
  configure_file("CMake/CTestCustom.cmake.in"
                 ${CMAKE_BINARY_DIR}/CTestCustom.cmake @ONLY)
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
  set(SHA_CXX_FLAGS "${cflags} ${SHA_CXX_FLAGS}")
#      -Woverloaded-virtual -Wold-style-cast ${SHA_CXX_FLAGS}")
#      -Wstrict-null-sentinel -Wsign-promo ${SHA_CXX_FLAGS}")
endif()

if(MSVC)
  set(msvc_suppressed_warnings
    "/wd4290" # C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
  )

  set(SHA_CXX_FLAGS "${SHA_CXX_FLAGS} ${msvc_suppressed_warnings}")

  # Add compiler flag for MSVC 2012
  # VS2012 doesn't support correctly the tuples used by GTest
  if(BUILD_TESTING AND MSVC_VERSION EQUAL 1700)
    add_definitions(-D_VARIADIC_MAX=10)
  endif()
endif()

add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE
                -D_CRT_SECURE_NO_WARNINGS
                -D_CRT_SECURE_NO_DEPRECATE)

#-----------------------------------------------------------------------------
# Set C/CXX Flags
#
set(CMAKE_CXX_FLAGS ${SHA_CXX_FLAGS} CACHE STRING "CMake C Flags" FORCE)
set(CMAKE_C_FLAGS ${SHA_C_FLAGS} CACHE STRING "CMake CXX Flags" FORCE)

# Force static runtime libraries to build against gtest
FOREACH(flag
  CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
  CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_DEBUG_INIT
  CMAKE_CXX_FLAGS_RELEASE  CMAKE_CXX_FLAGS_RELWITHDEBINFO
  CMAKE_CXX_FLAGS_DEBUG  CMAKE_CXX_FLAGS_DEBUG_INIT)

  if(MSVC)
    STRING(REPLACE "/MD"  "/MT" "${flag}" "${${flag}}")
    SET("${flag}" "${${flag}} /EHsc")
  else()
    SET("${flag}" "${${flag}} -std=c++11")
  endif()
ENDFOREACH()

#-----------------------------------------------------------------------------
# Add the modules
#
add_subdirectory(Modules)
