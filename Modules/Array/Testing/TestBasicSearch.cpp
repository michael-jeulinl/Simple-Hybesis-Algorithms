#include <gtest/gtest.h>
#include <Search.hpp>

namespace {
  const int SortedIntArray[] = {-3, -2, 0, 2, 8, 15,36, 212, 366}; // Simple sorted array of integers with negative values
  const float SortedFloatArray[] = {-.3, 0.0, 0.12, 2.5, 8};       // Simple sorted array of floats with negative values
}

// Basic BinarySearchIterative tests on a sorted array of ints
TEST(TestBasicSearch, BinarySearchIterativeTBasicInt)
{
  std::vector<int> sortedArray(SortedIntArray, SortedIntArray + sizeof(SortedIntArray) / sizeof(int));

  EXPECT_EQ(0, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, -3));  // Find first value
  EXPECT_EQ(4, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, 8));   // Find a random value owned by the vector
  EXPECT_EQ(-1, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, 1));  // Try to find a non-existing value
}

// Basic BinarySearchIterative tests on a sorted array of floats
TEST(TestBasicSearch, BinarySearchIterativeTBasicFloat)
{
  std::vector<float> sortedFloatArray(SortedFloatArray, SortedFloatArray + sizeof(SortedFloatArray) / sizeof(float));

  EXPECT_EQ(0, ArrayAlgorithms::BinarySearchIterativeT<float>(sortedFloatArray, static_cast<const float>(-.3)));    // Find first value
  EXPECT_EQ(2, ArrayAlgorithms::BinarySearchIterativeT<float>(sortedFloatArray, static_cast<const float>(0.12)));   // Find a random value owned by the vector
  EXPECT_EQ(-1, ArrayAlgorithms::BinarySearchIterativeT<float>(sortedFloatArray, static_cast<const float>(8.1)));   // Try to find a non-existing value
}

// Test BinarySearchIterative does not loop with a single array value
TEST(TestBasicSearch, BinarySearchIterativeTNoLoop)
{
  std::vector<int> sameValueArray(10, 0);

  EXPECT_EQ(4, ArrayAlgorithms::BinarySearchIterativeT<int>(sameValueArray, 0));    // Find the value in the middle
  EXPECT_EQ(-1, ArrayAlgorithms::BinarySearchIterativeT<int>(sameValueArray, 5));   // Should not find other value
}

// Test BinarySearchIterative does not fails on empty array
TEST(TestBasicSearch, BinarySearchIterativeTEmptyArray)
{
  EXPECT_EQ(-1, ArrayAlgorithms::BinarySearchIterativeT<int>(std::vector<int>(), 5));    // Should return -1 on empty array
}
