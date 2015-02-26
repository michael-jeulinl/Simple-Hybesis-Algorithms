#include <gtest/gtest.h>
#include <Search.hpp>

namespace {
  const int SortedArray[] = {-3, -2, 0, 2, 8, 15,36, 212, 366}; // Simple sorted array with negative values
}

TEST(TestBasicSearch, BinarySearchIterativeTBasic) 
{
  std::vector<int> sortedArray(SortedArray, SortedArray + sizeof(SortedArray) / sizeof(int));

  EXPECT_EQ(0, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, -3));  // Find first value
  EXPECT_EQ(4, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, 8));   // Find a random value owned by the vector
  EXPECT_EQ(-1, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, 1));  // Try to find a non-existing value
}