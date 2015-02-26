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

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
cxx_gtest(TestBasicSearch TestBasicSearch.cpp ${SHA_SRCS})
