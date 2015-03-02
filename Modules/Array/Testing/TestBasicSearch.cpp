#include <gtest/gtest.h>
#include <Search.hpp>

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15,36, 212, 366}; // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};// Simple random array of integers with negative values
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};     // Simple sorted array of floats with negative values

}

// Basic BinarySearchIterative tests on a sorted array of ints
TEST(TestBasicSearch, BinarySearchIterativeTBasicInt)
{
  const std::vector<int> sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

  EXPECT_EQ(-1, ArrayAlgorithms::BinarySearchIterativeT<int>(std::vector<int>(), 0)); // Should return -1 on empty array
  EXPECT_EQ(0, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, -3));        // Find first value
  EXPECT_EQ(4, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, 8));         // Find a random value owned by the vector
  EXPECT_EQ(-1, ArrayAlgorithms::BinarySearchIterativeT<int>(sortedArray, 1));        // Try to find a non-existing value
}

// Basic BinarySearchIterative tests on a sorted array of doubles
TEST(TestBasicSearch, BinarySearchIterativeTBasicDoubles)
{
  const std::vector<double> sortedDoubleArray(SortedDoubleArray, SortedDoubleArray + sizeof(SortedDoubleArray) / sizeof(double));

  EXPECT_EQ(0, ArrayAlgorithms::BinarySearchIterativeT<double>(sortedDoubleArray, static_cast<const double>(-.3)));   // Find first value
  EXPECT_EQ(2, ArrayAlgorithms::BinarySearchIterativeT<double>(sortedDoubleArray, static_cast<const double>(0.12)));  // Find a random value owned by the vector
  EXPECT_EQ(-1, ArrayAlgorithms::BinarySearchIterativeT<double>(sortedDoubleArray, static_cast<const double>(8.1)));  // Try to find a non-existing value
  EXPECT_EQ(4, ArrayAlgorithms::BinarySearchIterativeT<double>(std::vector<double>(10, 3.), 3.));                     // Find the value in the middle when identical values
}


// Test SimpleStockMarketProblem on a simple market array
TEST(TestBasicSearch, SimpleStockMarketProblem)
{
  // Should return <4,9> (largest benefice of 6)
  {
    const std::vector<int> marketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> maxBeneficeIndexes = ArrayAlgorithms::MaxDistance<int, std::minus<int> >(marketPrices);
    EXPECT_EQ(4, maxBeneficeIndexes.first);
    EXPECT_EQ(9, maxBeneficeIndexes.second);
  }

  // Should return <0, idxEnd> on sorted array
  {
    const std::vector<int> sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> indexes = ArrayAlgorithms::MaxDistance<int, std::minus<int> >(sortedArray);
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(static_cast<int>(sortedArray.size()) - 1, indexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const std::pair<int, int> indexes = ArrayAlgorithms::MaxDistance<int, std::minus<int> >(std::vector<int>(1, 2));
    EXPECT_EQ(-1, indexes.first);
    EXPECT_EQ(-1, indexes.second);
  }

  // Should return <0,1> on array containing only two elements
  {
    const std::pair<int, int> indexes = ArrayAlgorithms::MaxDistance<int, std::minus<int> >(std::vector<int>(2, 2));
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // Should return <0,1> on array containing the same value
  {
    const std::pair<int, int> indexes = ArrayAlgorithms::MaxDistance<int, std::minus<int> >(std::vector<int>(10, 2));
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }
}


// Test MaximalSubArray
TEST(TestBasicSearch, MaximalSubArray)
{
  // Should return <5,9> (maximal sum of 17)
  {
    const std::vector<int> kMarketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> kMaxBeneficeIndexes = ArrayAlgorithms::MaximalSubArray<int, std::minus<int>, std::greater<int> >(kMarketPrices);
    EXPECT_EQ(5, kMaxBeneficeIndexes.first);
    EXPECT_EQ(9, kMaxBeneficeIndexes.second);
  }

  // Should return <FirstPositiveIdx, idxEnd> on sorted array
  {
    const std::vector<int> kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> kIndexes = ArrayAlgorithms::MaximalSubArray<int, std::minus<int>, std::greater<int> >(kSortedArray);
    EXPECT_EQ(2, kIndexes.first);
    EXPECT_EQ(static_cast<int>(kSortedArray.size()) - 1, kIndexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const std::pair<int, int> kIndexes = ArrayAlgorithms::MaximalSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(1, 2));
    EXPECT_EQ(-1, kIndexes.first);
    EXPECT_EQ(-1, kIndexes.second);
  }

  // Should return <0,1> on array containing only two positive elements
  {
    const std::pair<int, int> kIndexes = ArrayAlgorithms::MaximalSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(2, 2));
    EXPECT_EQ(0, kIndexes.first);
    EXPECT_EQ(1, kIndexes.second);
  }

  // Should return <0, idxEnd> on array containing the same positive value
  {
    const int kSize = 10;
    const std::pair<int, int> indexes = ArrayAlgorithms::MaximalSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(kSize, 2));
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(kSize - 1, indexes.second);
  }
}
