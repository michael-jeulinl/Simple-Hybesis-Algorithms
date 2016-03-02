#include <gtest/gtest.h>
#include <sort.hxx>

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};       // Simple sorted array of integers with negative values
  const int SortedArrayIntWithRot[] = {-3, 2, 7, 20, 0, 2, 8, 15, 36};    // Simple sorted array of integers with negative values
  const int SortedArrayIntPos[] = {0, 2, 8, 15, 36, 212, 366, 15478};     // Simple sorted array of integers with positive values only
  const int InvSortedArrayInt[] = {366, 212, 36, 15, 8, 2, 0, -2, -3};    // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};      // Simple random array of integers with negative values
  const int RandomArrayIntPos[] = {4520, 30, 500, 20, 3, 2, 3, 4, 5, 15}; // Simple random array of integers with positive values only
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};            // Simple sorted array of floats with negative values
  const std::string RandomStr = "xacvgeze";                               // Random string
  const std::string StringWithPivot = "eknxasuw";                         // String with pivot at end =  begin() + 4 : Left sorted part [e,k,n,x] - Right sorted part [a,s,u,w]

  typedef std::vector<int> Container_type;
  typedef Container_type::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;
  typedef std::greater_equal<Iterator_type::value_type> Greater_Comparator_type;
  typedef std::vector<double>::const_iterator Const_Double_Iterator_Type;
  typedef SHA_Collections::MergeWithBuffer<Container_type, Iterator_type> Aggregator_type;
  typedef SHA_Collections::MergeWithBuffer<std::string, std::string::iterator> AggregatorStr_type;
}

// Basic Partition tests
TEST(TestSort, Partitions)
{
  // Normal Run - Random Array
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition - Should result in: max[begin, pivot[ <= pivot <= min]pivot, end]
    SHA_Collections::Partition<Iterator_type, Comparator_type>
      (randomdArray.begin(), pivot, randomdArray.end());

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
    Container_type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_type pivot = sortedArray.begin() + 5;

    SHA_Collections::Partition<Iterator_type, Comparator_type>
      (sortedArray.begin(), pivot, sortedArray.end());

    int i = 0;
    for (Iterator_type it = sortedArray.begin(); it < sortedArray.end(); ++it, ++i)
      EXPECT_EQ(SortedArrayInt[i], *it);
  }

  // Begin and End inversed - Array should not be affected
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin() + 5;

    SHA_Collections::Partition<Iterator_type, Comparator_type>
      (randomdArray.end(), pivot, randomdArray.begin());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }

  //String collection - Should result in: max[begin, pivot[ <= pivot <= min]pivot, end]
  {
    std::string randomStr = RandomStr;
    std::string::iterator pivot = randomStr.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    SHA_Collections::Partition<std::string::iterator, std::less_equal<char>>
      (randomStr.begin(), pivot, randomStr.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are smaller or equal
    for (std::string::iterator it = randomStr.begin(); it < pivot; ++it)
      EXPECT_GE(*pivot, *it);
    // All elements before the pivot are bigger or equal
    for (std::string::iterator it = pivot; it < randomStr.end(); ++it)
      EXPECT_LE(*pivot, *it);
  }
}

// Extreme Pivot Partition tests
TEST(TestSort, PartitionBoudaryPivots)
{
  // Pivot choose as begin
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin();
    const int pivotValue = *pivot;

    // Run partition
    SHA_Collections::Partition<Iterator_type, Comparator_type>
      (randomdArray.begin(), pivot, randomdArray.end());

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
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.end() - 1;
    const int pivotValue = *pivot;

    // Run partition
    SHA_Collections::Partition<Iterator_type, Comparator_type>
      (randomdArray.begin(), pivot, randomdArray.end());

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
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.end();

    // Run partition
    SHA_Collections::Partition<Iterator_type, Comparator_type>
      (randomdArray.begin(), pivot, randomdArray.end());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }
}

// Basic Partition tests - Greater element in the left partition
TEST(TestSort, PartitionGreaterComparator)
{
  // Normal Run - Should result in: min[begin, pivot[ >= pivot >= max]pivot, end]
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type pivot = randomdArray.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    SHA_Collections::Partition<Iterator_type, Greater_Comparator_type>
      (randomdArray.begin(), pivot, randomdArray.end());

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
    Container_type invSortedArray
      (InvSortedArrayInt, InvSortedArrayInt + sizeof(InvSortedArrayInt) / sizeof(int));
    Iterator_type pivot = invSortedArray.begin() + 5;

    SHA_Collections::Partition<Iterator_type, Greater_Comparator_type>
      (invSortedArray.begin(), pivot, invSortedArray.end());

    int i = 0;
    for (Iterator_type it = invSortedArray.begin(); it < invSortedArray.end(); ++it, ++i)
      EXPECT_EQ(invSortedArray[i], *it);
  }

  // String collection - Should result in: min[begin, pivot[ >= pivot >= max]pivot, end]
  {
    std::string randomStr = RandomStr;
    std::string::iterator pivot = randomStr.begin() + 5;
    const int pivotValue = *pivot;

    // Run partition
    SHA_Collections::Partition<std::string::iterator, std::greater_equal<char>>
      (randomStr.begin(), pivot, randomStr.end());

    // Value of the pivot no changed
    EXPECT_EQ(pivotValue, *pivot);
    // All elements before the pivot are greater or equal
    for (std::string::iterator it = randomStr.begin(); it < pivot; ++it)
      EXPECT_LE(*pivot, *it);
    // All elements before the pivot are less or equal
    for (std::string::iterator it = pivot; it < randomStr.end(); ++it)
      EXPECT_GE(*pivot, *it);
  }
}

// Basic Quick-Sort tests
TEST(TestSort, QuickSorts)
{
  // Normal Run
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    SHA_Collections::QuickSort<Iterator_type, Comparator_type>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    SHA_Collections::QuickSort<Iterator_type, Comparator_type>(sortedArray.begin(), sortedArray.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArray.begin(); it < sortedArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    SHA_Collections::QuickSort<Iterator_type, Comparator_type>(randomdArray.end(), randomdArray.begin());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }

  // No error unitialized array
  {
    Container_type emptyArray;
    SHA_Collections::QuickSort<Iterator_type, Comparator_type>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    SHA_Collections::QuickSort<Iterator_type, Comparator_type>
      (uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // String - String should be sorted as an array
  {
    std::string stringToSort = RandomStr;
    SHA_Collections::QuickSort<std::string::iterator, std::less_equal<char>>
      (stringToSort.begin(), stringToSort.end());
    EXPECT_EQ('a', stringToSort[0]);
    EXPECT_EQ('c', stringToSort[1]);
    EXPECT_EQ('e', stringToSort[2]);
    EXPECT_EQ('e', stringToSort[3]);
    EXPECT_EQ('g', stringToSort[4]);
    EXPECT_EQ('v', stringToSort[5]);
    EXPECT_EQ('x', stringToSort[6]);
    EXPECT_EQ('z', stringToSort[7]);
  }
}

// Basic Quick-Sort tests - Inverse Order
TEST(TestSort, QuickSortGreaterComparator)
{
  // Normal Run - Elements should be sorted in inverse order
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    SHA_Collections::QuickSort<Iterator_type, Greater_Comparator_type>
      (randomdArray.begin(), randomdArray.end());

    // All elements are sorted in inverse order
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // Already sorted Array in inverse order - Array should not be affected
  {
    Container_type invSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    SHA_Collections::QuickSort<Iterator_type, Greater_Comparator_type>
      (invSortedArray.begin(), invSortedArray.end());

    // All elements are still sorted in inverse order
    for (Iterator_type it = invSortedArray.begin(); it < invSortedArray.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // String - String should be sorted in inverse order
  {
    std::string stringToSort = RandomStr;
    SHA_Collections::QuickSort<std::string::iterator, std::greater_equal<char>>
      (stringToSort.begin(), stringToSort.end());

    // All elements are sorted in inverse order
    for (std::string::iterator it = stringToSort.begin(); it < stringToSort.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }
}

// Basic Quick-Sort tests
TEST(TestSort, RaddixSorts)
{
  // Normal Run - array should be sorted in order
  {
    Container_type randomdArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    SHA_Collections::RaddixSort<Iterator_type>(randomdArrayPos.begin(), randomdArrayPos.end());

    // All elements are sorted
    for (Iterator_type it = randomdArrayPos.begin(); it < randomdArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArrayPos
      (SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    SHA_Collections::RaddixSort<Iterator_type>(sortedArrayPos.begin(), sortedArrayPos.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    SHA_Collections::RaddixSort<Iterator_type>(randomArrayPos.end(), randomArrayPos.begin());

    int i = 0;
    for (Iterator_type it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container_type emptyArray;
    SHA_Collections::RaddixSort<Iterator_type>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    SHA_Collections::RaddixSort<Iterator_type>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }
}

// Basic MergeInPlace tests
TEST(TestSort, MergeInPlaces)
{
  // Normal Run - All elements should be sorted in order
  {
    Container_type sortedArrayWithRot
      (SortedArrayIntWithRot, SortedArrayIntWithRot + sizeof(SortedArrayIntWithRot) / sizeof(int));
    SHA_Collections::MergeInPlace<Iterator_type>()
      (sortedArrayWithRot.begin(), sortedArrayWithRot.begin() + 4, sortedArrayWithRot.end());

    // All elements of the final array are sorted
    for (Iterator_type it = sortedArrayWithRot.begin(); it < sortedArrayWithRot.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArrayPos
      (SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    SHA_Collections::MergeInPlace<Iterator_type>()
      (sortedArrayPos.begin(), sortedArrayPos.begin() + 5, sortedArrayPos.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    SHA_Collections::MergeInPlace<Iterator_type>()
      (randomArrayPos.end(), randomArrayPos.begin() + 3, randomArrayPos.begin());

    int i = 0;
    for (Iterator_type it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container_type emptyArray;
    SHA_Collections::MergeInPlace<Iterator_type>()(emptyArray.begin(), emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    SHA_Collections::MergeInPlace<Iterator_type>()
      (uniqueValueArray.begin(), uniqueValueArray.end(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // Double values array - Order should be made
  {
    Container_type doubleValuesArray(1, 511);
    doubleValuesArray.push_back(66);

    SHA_Collections::MergeInPlace<Iterator_type>()
      (doubleValuesArray.begin(), doubleValuesArray.begin() + 1, doubleValuesArray.end());

    EXPECT_EQ(66, doubleValuesArray[0]);
    EXPECT_EQ(511, doubleValuesArray[1]);
  }

  // String Collection - All elements should be sorted in order
  {
    std::string stringToMerge = StringWithPivot;
    SHA_Collections::MergeInPlace<std::string::iterator>()
      (stringToMerge.begin(), stringToMerge.begin() + 4, stringToMerge.end());

    // All elements of the final array are sorted
    for (std::string::iterator it = stringToMerge.begin(); it < stringToMerge.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}


// Basic MergeWithBuffer tests
TEST(TestSort, MergeWithBuffers)
{
  // Normal Run - All elements should be sorted in order
  {
    Container_type sortedArrayWithRot
      (SortedArrayIntWithRot, SortedArrayIntWithRot + sizeof(SortedArrayIntWithRot) / sizeof(int));
    SHA_Collections::MergeWithBuffer<Container_type, Iterator_type>()
      (sortedArrayWithRot.begin(), sortedArrayWithRot.begin() + 4, sortedArrayWithRot.end());

    // All elements of the final array are sorted
    for (Iterator_type it = sortedArrayWithRot.begin(); it < sortedArrayWithRot.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArrayPos
      (SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    SHA_Collections::MergeWithBuffer<Container_type, Iterator_type>()
      (sortedArrayPos.begin(), sortedArrayPos.begin() + 5, sortedArrayPos.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    SHA_Collections::MergeWithBuffer<Container_type, Iterator_type>()
      (randomArrayPos.end(), randomArrayPos.begin() + 3, randomArrayPos.begin());

    int i = 0;
    for (Iterator_type it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container_type emptyArray;
    SHA_Collections::MergeWithBuffer<Container_type, Iterator_type>()
      (emptyArray.begin(), emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    SHA_Collections::MergeWithBuffer<Container_type, Iterator_type>()
      (uniqueValueArray.begin(), uniqueValueArray.end(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // Double values array - Order should be made
  {
    Container_type doubleValuesArray(1, 511);
    doubleValuesArray.push_back(66);

    SHA_Collections::MergeWithBuffer<Container_type, Iterator_type>()
      (doubleValuesArray.begin(), doubleValuesArray.begin() + 1, doubleValuesArray.end());

    EXPECT_EQ(66, doubleValuesArray[0]);
    EXPECT_EQ(511, doubleValuesArray[1]);
  }

  // String Collection - All elements should be sorted in order
  {
    std::string stringToMerge = "eknxasuw"; // Left sorted part [e,k,n,x] - Right sorted part [a,s,u,w]
    SHA_Collections::MergeWithBuffer<std::string, std::string::iterator>()
      (stringToMerge.begin(), stringToMerge.begin() + 4, stringToMerge.end());

    // All elements of the final array are sorted
    for (std::string::iterator it = stringToMerge.begin(); it < stringToMerge.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Basic Merge-Sort tests - Uses the merge with buffer (computation optimized)
TEST(TestSort, MergeSorts)
{
  // Normal Run - all elements should be sorter in order
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    SHA_Collections::MergeSort<Container_type, Iterator_type, Aggregator_type>
      (randomdArray.begin(), randomdArray.end());

    // All elements are sorted
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container_type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    SHA_Collections::MergeSort<Container_type, Iterator_type, Aggregator_type>
      (sortedArray.begin(), sortedArray.end());

    // All elements are still sorted
    for (Iterator_type it = sortedArray.begin(); it < sortedArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container_type randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    SHA_Collections::MergeSort<Container_type, Iterator_type, Aggregator_type>
      (randomdArray.end(), randomdArray.begin());

    int i = 0;
    for (Iterator_type it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }

  // No error empty array
  {
    Container_type emptyArray;
    SHA_Collections::MergeSort<Container_type, Iterator_type, Aggregator_type>
      (emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container_type uniqueValueArray(1, 511);
    SHA_Collections::MergeSort<Container_type, Iterator_type, Aggregator_type>
      (uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // String collection - all elements should be sorter in order
  {
    std::string randomStr = RandomStr;
    SHA_Collections::MergeSort<std::string, std::string::iterator, AggregatorStr_type>
      (randomStr.begin(), randomStr.end());

    // All elements are sorted
    for (std::string::iterator it = randomStr.begin(); it < randomStr.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}
