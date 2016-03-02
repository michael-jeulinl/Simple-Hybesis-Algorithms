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
set(MODULE_COLLECTIONS_SRCS TestCore.cxx
                            TestSearch.cxx
                            TestSort.cxx)

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
cxx_gtest(TestModuleCollections "${MODULE_COLLECTIONS_SRCS}" ${SHA_SRCS})
