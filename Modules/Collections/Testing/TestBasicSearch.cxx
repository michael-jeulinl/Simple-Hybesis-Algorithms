#include <gtest/gtest.h>
#include <search.hxx>

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366}; // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};// Simple random array of integers with negative values
  const int RandomArrayInterInt[] = {5 , 5, -5, 3, -18, 10, 15};    // Other  random array of integers with negative values
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};      // Simple sorted array of floats with negative values

}

// Basic BinarySearchIterative tests on a sorted array of ints
TEST(TestBasicSearch, BinarySearchBasicInt)
{
  const std::vector<int> sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

  EXPECT_EQ(-1, Collections::BinarySearch<int>(std::vector<int>(), 0)); // Should return -1 on empty array
  EXPECT_EQ(0, Collections::BinarySearch<int>(sortedArray, -3));        // Find first value
  EXPECT_EQ(4, Collections::BinarySearch<int>(sortedArray, 8));         // Find a random value owned by the vector
  EXPECT_EQ(-1, Collections::BinarySearch<int>(sortedArray, 1));        // Try to find a non-existing value
}

// Basic BinarySearchIterative tests on a sorted array of doubles
TEST(TestBasicSearch, BinarySearchBasicDoubles)
{
  const std::vector<double> sortedDoubleArray(SortedDoubleArray, SortedDoubleArray + sizeof(SortedDoubleArray) / sizeof(double));

  EXPECT_EQ(0, Collections::BinarySearch<double>(sortedDoubleArray, static_cast<const double>(-.3)));   // Find first value
  EXPECT_EQ(2, Collections::BinarySearch<double>(sortedDoubleArray, static_cast<const double>(0.12)));  // Find a random value owned by the vector
  EXPECT_EQ(-1, Collections::BinarySearch<double>(sortedDoubleArray, static_cast<const double>(8.1)));  // Try to find a non-existing value
  EXPECT_EQ(4, Collections::BinarySearch<double>(std::vector<double>(10, 3.), 3.));                     // Find the value in the middle when identical values
}


// Test SimpleStockMarketProblem on a simple market array
TEST(TestBasicSearch, SimpleStockMarketProblem)
{
  // Should return <4,9> (largest benefice of 6)
  {
    const std::vector<int> marketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> maxBeneficeIndexes = Collections::MaxDistance<int, std::minus<int> >(marketPrices);
    EXPECT_EQ(4, maxBeneficeIndexes.first);
    EXPECT_EQ(9, maxBeneficeIndexes.second);
  }

  // Should return <0, idxEnd> on sorted array
  {
    const std::vector<int> sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> indexes = Collections::MaxDistance<int, std::minus<int> >(sortedArray);
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(static_cast<int>(sortedArray.size()) - 1, indexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const std::pair<int, int> indexes = Collections::MaxDistance<int, std::minus<int> >(std::vector<int>(1, 2));
    EXPECT_EQ(-1, indexes.first);
    EXPECT_EQ(-1, indexes.second);
  }

  // Should return <0,1> on array containing only two elements
  {
    const std::pair<int, int> indexes = Collections::MaxDistance<int, std::minus<int> >(std::vector<int>(2, 2));
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // Should return <0,1> on array containing the same value
  {
    const std::pair<int, int> indexes = Collections::MaxDistance<int, std::minus<int> >(std::vector<int>(10, 2));
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }
}


// Test MaxSubArray
TEST(TestBasicSearch, MaxSubArray)
{
  // Should return <5,9> (maximal sum of 17)
  {
    const std::vector<int> kMarketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> kMaxBeneficeIndexes = Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(kMarketPrices);
    EXPECT_EQ(5, kMaxBeneficeIndexes.first);
    EXPECT_EQ(9, kMaxBeneficeIndexes.second);
  }

  // Should return <FirstPositiveIdx, idxEnd> on sorted array
  {
    const std::vector<int> kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> kIndexes = Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(kSortedArray);
    EXPECT_EQ(2, kIndexes.first);
    EXPECT_EQ(static_cast<int>(kSortedArray.size()) - 1, kIndexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const std::pair<int, int> kIndexes = Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(1, 2));
    EXPECT_EQ(-1, kIndexes.first);
    EXPECT_EQ(-1, kIndexes.second);
  }

  // Should return <0,1> on array containing only two positive elements
  {
    const std::pair<int, int> kIndexes = Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(2, 2));
    EXPECT_EQ(0, kIndexes.first);
    EXPECT_EQ(1, kIndexes.second);
  }

  // Should return <0, idxEnd> on array containing the same positive value
  {
    const int kSize = 10;
    const std::pair<int, int> indexes = Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(kSize, 2));
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(kSize - 1, indexes.second);
  }
}

// Test MaxNElements
TEST(TestBasicSearch, MaxNElements)
{
  // Should return the max value [5] for a unique element search
  {
    const std::vector<int> kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = Collections::MaxMElements<int, std::greater_equal<int> >(kRandomElements, 1);
    EXPECT_EQ(5, kMMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    const std::vector<int> kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = Collections::MaxMElements<int, std::greater_equal<int> >(kRandomElements, 3);
    EXPECT_EQ(5, kMMaxElements[0]);
    EXPECT_EQ(5, kMMaxElements[1]);
    EXPECT_EQ(4, kMMaxElements[2]);
  }

  // Should return the last elements on sorted vector
  {
    const std::vector<int> kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = Collections::MaxMElements<int, std::greater_equal<int> >(kSortedArray, 4);
    EXPECT_EQ(366, kMMaxElements[0]);
    EXPECT_EQ(212, kMMaxElements[1]);
    EXPECT_EQ(36, kMMaxElements[2]);
    EXPECT_EQ(15, kMMaxElements[3]);
  }

  // Should return empty vector on insufficient vector
  {
    const std::vector<int> kMMaxElements = Collections::MaxMElements<int, std::greater_equal<int> >(std::vector<int>(1, 2), 2);
    EXPECT_EQ(0, static_cast<int>(kMMaxElements.size()));
  }

  // Should return empty vector when looking for less than 1 elements
  {
    const std::vector<int> kMMaxElements = Collections::MaxMElements<int, std::greater_equal<int> >(std::vector<int>(1, 2), 0);
    EXPECT_EQ(0, static_cast<int>(kMMaxElements.size()));
  }
}

// Test MaxNElements to find the lowest values
TEST(TestBasicSearch, MaxNElementsLowestValues)
{
  // Should return the min value [-18] for a unique element search
  {
    const std::vector<int> kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = Collections::MaxMElements<int, std::less_equal<int> >(kRandomElements, 1);
    EXPECT_EQ(-18, kMMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    const std::vector<int> kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = Collections::MaxMElements<int, std::less_equal<int> >(kRandomElements, 3);
    EXPECT_EQ(-18, kMMaxElements[0]);
    EXPECT_EQ(-5, kMMaxElements[1]);
    EXPECT_EQ(2, kMMaxElements[2]);
  }

  // Should return the first elements on sorted vector
  {
    const std::vector<int> kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = Collections::MaxMElements<int, std::less_equal<int> >(kSortedArray, 4);
    EXPECT_EQ(-3, kMMaxElements[0]);
    EXPECT_EQ(-2, kMMaxElements[1]);
    EXPECT_EQ(0, kMMaxElements[2]);
    EXPECT_EQ(2, kMMaxElements[3]);
  }
}

// Test kth smallest element
TEST(TestBasicSearch, KthSmallesElement)
{
  typedef std::vector<int>::iterator Iterator_type;
  typedef std::less_equal<Iterator_type::value_type> Comparator_type;

  // Basic run on sorted array with unique element - Should the kth element
  {
    std::vector<int> ksortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_type it = Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.end(), 4);
    EXPECT_EQ(ksortedArray.begin() + 4, it);
  }

  // Basic run on random array - Should return 4
  {
    std::vector<int> krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type::value_type value = *Collections::KthMaxElement<Iterator_type, Comparator_type>(krandomdArray.begin(), krandomdArray.end(), 7);
    EXPECT_EQ(4, value);
  }

  // Basic run with extreme values
  {
    std::vector<int> ksortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_type it;

    it = Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.begin(), 0);
    EXPECT_EQ(ksortedArray.begin(), it);     // Should return end on empty sequence
    it = Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.begin() + 1, 0);
    EXPECT_EQ(ksortedArray.begin(), it);     // Should return the unique element
    it = Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.end(), -1);
    EXPECT_EQ(ksortedArray.end(), it);       // Should end for out of scope k (-1)
    it =  Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.end(), 100);
    EXPECT_EQ(ksortedArray.end(), it);       // Should end for out of scope k (100)
  }
}

// Test kth biggest element
TEST(TestBasicSearch, KthBiggestElement)
{
  typedef std::vector<int>::iterator Iterator_type;
  typedef std::greater_equal<Iterator_type::value_type> Comparator_type;

  // Basic run on random array - Should return 5 (second biggest value)
  {
    std::vector<int> krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type::value_type value = *Collections::KthMaxElement<Iterator_type, Comparator_type>(krandomdArray.begin(), krandomdArray.end(), 1);
    EXPECT_EQ(5, value);
  }
}

// Test intersections
TEST(TestBasicSearch, BasicIntersections)
{
  typedef std::vector<int> Vector_type;

  EXPECT_EQ(0, Collections::Intersection(Vector_type(), Vector_type()).size());    // Null intersection on empty vectors
  EXPECT_EQ(0, Collections::Intersection(Vector_type(1,1), Vector_type()).size()); // Null intersection with empty vector

  // Basic run with same vector
  {
    const Vector_type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

    Vector_type intersectionVector = Collections::Intersection<int>(sortedArray, sortedArray);
    Vector_type::const_iterator intersectIt = intersectionVector.begin();

    // The intersection of the same vector should return the same vector
    ASSERT_EQ(sortedArray.size(), intersectionVector.size());
    for (Vector_type::const_iterator it = sortedArray.begin(); it != sortedArray.end(); ++it, ++intersectIt)
      EXPECT_EQ(*it, *intersectIt);
  }

  // Basic run with copy vector containning dupplicates
  {
    const Vector_type firstRandomVector(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const Vector_type secondRandomVector(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));

    Vector_type intersectionVector = Collections::Intersection<int>(firstRandomVector, secondRandomVector);
    Vector_type::const_iterator intersectIt = intersectionVector.begin();

    // The intersection with the copy vector should return the same vector
    ASSERT_EQ(firstRandomVector.size(), intersectionVector.size());
    for (Vector_type::const_iterator it = firstRandomVector.begin(); it != firstRandomVector.end(); ++it, ++intersectIt)
      EXPECT_EQ(*it, *intersectIt);
  }

  // Basic run with normal values
  {
    const Vector_type firstRandomVector(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const Vector_type secondRandomVector(RandomArrayInterInt, RandomArrayInterInt + sizeof(RandomArrayInterInt) / sizeof(int));

    Vector_type intersectionVector = Collections::Intersection<int>(firstRandomVector, secondRandomVector);

    // Should return [-18, -5, 3, 5, 5] after sorting
    std::sort(intersectionVector.begin(), intersectionVector.end());
    EXPECT_EQ(-18, intersectionVector[0]);
    EXPECT_EQ(-5, intersectionVector[1]);
    EXPECT_EQ(3, intersectionVector[2]);
    EXPECT_EQ(5, intersectionVector[3]);
    EXPECT_EQ(5, intersectionVector[4]);
  }
}
