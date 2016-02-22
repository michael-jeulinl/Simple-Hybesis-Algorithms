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

set(SHA ${PROJECT_NAME})

# Source files
set(MODULE_ARRAY_SRCS TestBasicSearch.cxx
                      TestBasicSort.cxx)

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
message("${MODULE_ARRAY_SRCS}")
cxx_gtest(TestModuleArray "${MODULE_ARRAY_SRCS}" ${SHA_SRCS})
