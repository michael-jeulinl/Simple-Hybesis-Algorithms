#include <gtest/gtest.h>
#include <sort.hxx>

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};       // Simple sorted array of floats with negative values

}

// Basic Partition tests
TEST(TestBasicSort, PartitionBasicVectorInt)
{
  // Normal Run
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    std::vector<int>::iterator pivot = randomdArray.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    ArrayAlgorithms::Partition<std::vector<int>::iterator>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (std::vector<int>::iterator it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (std::vector<int>::iterator it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Already sortedArray - Array should not be affected
  {
    std::vector<int> sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::vector<int> sortedArraySrc(sortedArray);
    std::vector<int>::iterator pivot = sortedArray.begin() + 5;

    ArrayAlgorithms::Partition<std::vector<int>::iterator>(sortedArray.begin(), pivot, sortedArray.end());

    int i = 0;
    for (std::vector<int>::iterator it = sortedArray.begin(); it < sortedArray.end(); ++it, ++i)
      EXPECT_EQ(sortedArraySrc[i], *it);
  }

  // Begin and End inversed - Array should not be affected
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> randomdArraySrc(randomdArray);
    std::vector<int>::iterator pivot = randomdArray.begin() + 5;

    ArrayAlgorithms::Partition<std::vector<int>::iterator>(randomdArray.end(), pivot, randomdArray.begin());

    int i = 0;
    for (std::vector<int>::iterator it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(randomdArraySrc[i], *it);
  }
}

// Extreme Pivot Partition tests
TEST(TestBasicSort, PartitionXTremePivotVectorInt)
{
  // Pivot choose as begin
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    std::vector<int>::iterator pivot = randomdArray.begin();
    const int pivotValue = *pivot;

    // Run partition
    ArrayAlgorithms::Partition<std::vector<int>::iterator>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (std::vector<int>::iterator it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (std::vector<int>::iterator it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Pivot choose as last element
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    std::vector<int>::iterator pivot = randomdArray.end() - 1;
    const int pivotValue = *pivot;

    // Run partition
    ArrayAlgorithms::Partition<std::vector<int>::iterator>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (std::vector<int>::iterator it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (std::vector<int>::iterator it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Pivot choose as end - cannot process
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    std::vector<int> randomdArraySrc(randomdArray);
    std::vector<int>::iterator pivot = randomdArray.end();

    // Run partition
    ArrayAlgorithms::Partition<std::vector<int>::iterator>(randomdArray.begin(), pivot, randomdArray.end());

    int i = 0;
    for (std::vector<int>::iterator it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(randomdArraySrc[i], *it);
  }
}
