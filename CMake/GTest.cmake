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

# Downloads GTest at compile time using git. 
# It then compiles it.

find_package(Threads)
include(ExternalProject)

ExternalProject_Add(gtest_ext
  GIT_REPOSITORY "https://git.chromium.org/git/external/googletest.git"
  BINARY_DIR "${CMAKE_BINARY_DIR}/gtest-build"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/gtest-src"
  INSTALL_COMMAND ""
  CMAKE_ARGS "${gtest_cmake_args}"
             "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
)

set(GTEST_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/gtest-src/include")
#set(GTEST_SRC_DIRS "${CMAKE_BINARY_DIR}/gtest-src")
message(${GTEST_INCLUDE_DIRS})
enable_testing()
function(cxx_gtest name sources sourcesDirectory)
  add_executable(${name} ${sources})
  add_dependencies(${name} gtest_ext)

  #target_link_libraries(
  #  ${name} ${ARGN}
  #  debug ${GTEST_SRC_DIRS}/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtest
  #  optimized ${GTEST_SRC_DIRS}/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest
  #  ${Pthread})

  target_link_libraries(${name} ${ARGN} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})
  set_property(TARGET ${name} APPEND PROPERTY INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS} ${sourcesDirectory})
  add_test(NAME ${name} COMMAND ${name} "--gtest_break_on_failure")
endfunction()