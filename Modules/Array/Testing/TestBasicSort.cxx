#include <gtest/gtest.h>
#include <sort.hxx>

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};       // Simple sorted array of integers with negative values
  const int SortedArrayIntPos[] = {0, 2, 8, 15, 36, 212, 366, 15478};     // Simple sorted array of integers with positive values only
  const int InvSortedArrayInt[] = {366, 212, 36, 15, 8, 2, 0, -2, -3};    // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};      // Simple random array of integers with negative values
  const int RandomArrayIntPos[] = {4520, 30, 500, 20, 3, 2, 3, 4, 5, 15}; // Simple random array of integers with positive values only
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};            // Simple sorted array of floats with negative values

}

// Basic Partition tests
TEST(TestBasicSort, PartitionBasicVectorInOrderInt)
{
  typedef std::vector<int>::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;

  // Normal Run
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    ArrayAlgorithms::Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (Iterator_type it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (Iterator_type it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Already sortedArray - Array should not be affected
  {
    std::vector<int> sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::vector<int> sortedArraySrc(sortedArray);
    Iterator_type pivot = sortedArray.begin() + 5;

    ArrayAlgorithms::Partition<Iterator_type, Comparator_type>(sortedArray.begin(), pivot, sortedArray.end());

    int i = 0;
    for (Iterator_type it = sortedArray.begin(); it < sortedArray.end(); ++it, ++i)
      EXPECT_EQ(sortedArraySrc[i], *it);
  }

  // Begin and End inversed - Array should not be affected
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> randomdArraySrc(randomdArray);
    Iterator_type pivot = randomdArray.begin() + 5;

    ArrayAlgorithms::Partition<Iterator_type, Comparator_type>(randomdArray.end(), pivot, randomdArray.begin());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(randomdArraySrc[i], *it);
  }
}

// Extreme Pivot Partition tests
TEST(TestBasicSort, PartitionInOrderXTremePivotVectorInt)
{
  typedef std::vector<int>::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;

  // Pivot choose as begin
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin();
    const int pivotValue = *pivot;

    // Run partition
    ArrayAlgorithms::Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (Iterator_type it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (Iterator_type it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Pivot choose as last element
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.end() - 1;
    const int pivotValue = *pivot;

    // Run partition
    ArrayAlgorithms::Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (Iterator_type it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (Iterator_type it = pivot; it < randomdArray.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }

  // Pivot choose as end - cannot process
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    std::vector<int> randomdArraySrc(randomdArray);
    Iterator_type pivot = randomdArray.end();

    // Run partition
    ArrayAlgorithms::Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(randomdArraySrc[i], *it);
  }
}

// Basic Partition tests - Greater element in the left partition
TEST(TestBasicSort, PartitionBasicVectorInverseOrderInt)
{
  typedef std::vector<int>::iterator Iterator_type;
  typedef std::greater_equal<Iterator_type::value_type> Comparator_type;

  // Normal Run
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    ArrayAlgorithms::Partition<Iterator_type, Comparator_type>(randomdArray.begin(), pivot, randomdArray.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are greater or equal
    for (Iterator_type it = randomdArray.begin(); it < pivot; ++it)
      EXPECT_LE(*pivot, *it);
    // All elements before the pivot are less or equal
    for (Iterator_type it = pivot; it < randomdArray.end(); ++it)
      EXPECT_GE(*pivot, *it);
  }

  // Already InverseSortedArray - Array should not be affected
  {
    std::vector<int> invSortedArray(InvSortedArrayInt, InvSortedArrayInt + sizeof(InvSortedArrayInt) / sizeof(int));
    const std::vector<int> invSortedArraySrc(invSortedArray);
    Iterator_type pivot = invSortedArray.begin() + 5;

    ArrayAlgorithms::Partition<Iterator_type, Comparator_type>(invSortedArray.begin(), pivot, invSortedArray.end());

    int i = 0;
    for (Iterator_type it = invSortedArray.begin(); it < invSortedArray.end(); ++it, ++i)
      EXPECT_EQ(invSortedArray[i], *it);
  }
}

// Basic Quick-Sort tests
TEST(TestBasicSort, QuickSortBasicVectorInt)
{
  typedef std::vector<int>::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;

  // Normal Run
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));

    // Run Quick-Sort
    ArrayAlgorithms::QuickSort<Iterator_type, Comparator_type>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    std::vector<int> sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

    ArrayAlgorithms::QuickSort<Iterator_type, Comparator_type>(sortedArray.begin(), sortedArray.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArray.begin(); it < sortedArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    std::vector<int> randomdArraySrc(randomdArray);

    // Run Quick-Sort
    ArrayAlgorithms::QuickSort<Iterator_type, Comparator_type>(randomdArray.end(), randomdArray.begin());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(randomdArraySrc[i], *it);
  }

  // No error empty array
  {
    std::vector<int> emptyArray;
    ArrayAlgorithms::QuickSort<Iterator_type, Comparator_type>(emptyArray.begin(), emptyArray.end());

  }

  // Unique value array - Array should not be affected
  {
    std::vector<int> uniqueValueArray(1, 511);
    ArrayAlgorithms::QuickSort<Iterator_type, Comparator_type>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }
}

// Basic Quick-Sort tests - Inverse Order
TEST(TestBasicSort, QuickSortBasicInverseOrderVectorInt)
{
  typedef std::vector<int>::iterator Iterator_type;
  typedef std::greater_equal<Iterator_type::value_type> Comparator_type;

  // Normal Run
  {
    std::vector<int> randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));

    // Run Quick-Sort
    ArrayAlgorithms::QuickSort<Iterator_type, Comparator_type>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted in inverse order
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // Already sorted Array in inverse order - Array should not be affected
  {
    std::vector<int> invSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

    ArrayAlgorithms::QuickSort<Iterator_type, Comparator_type>(invSortedArray.begin(), invSortedArray.end());

    // All elements are still sorted in inverse order
    for (Iterator_type it = invSortedArray.begin(); it < invSortedArray.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }
}

// Basic Quick-Sort tests
TEST(TestBasicSort, RaddixSortBasicVector)
{
  typedef std::vector<int>::iterator Iterator_type;

  // Normal Run
  {
    std::vector<int> randomdArrayPos(RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));

    // Run Quick-Sort
    ArrayAlgorithms::RaddixSort<Iterator_type>(randomdArrayPos.begin(), randomdArrayPos.end());

    // All elements are sorted
    for (Iterator_type it = randomdArrayPos.begin(); it < randomdArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    std::vector<int> sortedArrayPos(SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));

    ArrayAlgorithms::RaddixSort<Iterator_type>(sortedArrayPos.begin(), sortedArrayPos.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    std::vector<int> randomArrayPos(RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    std::vector<int> randomdArraySrc(randomArrayPos);

    // Run Quick-Sort
    ArrayAlgorithms::RaddixSort<Iterator_type>(randomArrayPos.end(), randomArrayPos.begin());

    int i = 0;
    for (Iterator_type it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(randomdArraySrc[i], *it);
  }

  // No error empty array
  {
    std::vector<int> emptyArray;
    ArrayAlgorithms::RaddixSort<Iterator_type>(emptyArray.begin(), emptyArray.end());

  }

  // Unique value array - Array should not be affected
  {
    std::vector<int> uniqueValueArray(1, 511);
    ArrayAlgorithms::RaddixSort<Iterator_type>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }
}
