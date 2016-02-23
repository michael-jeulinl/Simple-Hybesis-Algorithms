#include <gtest/gtest.h>
#include <search.hxx>

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366}; // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};// Simple random array of integers with negative values
  const int RandomArrayInterInt[] = {5 , 5, -5, 3, -18, 10, 15};    // Other  random array of integers with negative values
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};      // Simple sorted array of floats with negative values
  const std::string OrderedStr = "acegmnoop";

  typedef std::vector<int> Conainer_Type;
  typedef Conainer_Type::const_iterator Const_Iterator_Type;
  typedef std::vector<double>::const_iterator Const_Double_Iterator_Type;
}

// Basic BinarySearchIterative tests on a sorted array of ints
TEST(TestBasicSearch, BinarySearchBasics)
{
  const Conainer_Type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

  // Empty array
  {
    Conainer_Type emptyArray = std::vector<int>();
    const int index = SHA_Collections::BinarySearch<Const_Iterator_Type, int>(emptyArray.begin(), emptyArray.end(), 0);
    EXPECT_EQ(-1, index); // Should return -1 on empty array
  }

  // First element
  {
    const int index = SHA_Collections::BinarySearch<Const_Iterator_Type, int>(sortedArray.begin(), sortedArray.end(), -3);
    EXPECT_EQ(0, index);
  }

  // Existing random value
  {
    const int index = SHA_Collections::BinarySearch<Const_Iterator_Type, int>(sortedArray.begin(), sortedArray.end(), 8);
    EXPECT_EQ(4, index);
  }

  // Non-existing element
  {
    const int index = SHA_Collections::BinarySearch<Const_Iterator_Type, int>(sortedArray.begin(), sortedArray.end(), 1);
    EXPECT_EQ(-1, index);
  }

  // String collection - Find letter
  {
    const int index = SHA_Collections::BinarySearch<std::string::const_iterator, char>(OrderedStr.begin(), OrderedStr.end(), 'm');
    EXPECT_EQ(4, index);
  }
}

// Basic BinarySearchIterative tests on a sorted array of doubles
TEST(TestBasicSearch, BinarySearchBasicDoubles)
{
  const std::vector<double> sortedDoubleArray(SortedDoubleArray, SortedDoubleArray + sizeof(SortedDoubleArray) / sizeof(double));

  // First element
  {
    const int index = SHA_Collections::BinarySearch<Const_Double_Iterator_Type, double>(sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(-.3));
    EXPECT_EQ(0, index);
  }

  // Existing element
  {
    const int index = SHA_Collections::BinarySearch<Const_Double_Iterator_Type, double>(sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(0.12));
    EXPECT_EQ(2, index);
  }

  // Non Existing element
  {
    const int index = SHA_Collections::BinarySearch<Const_Double_Iterator_Type, double>(sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(8.1));
    EXPECT_EQ(-1, index);
  }

  // Value in the middle when identical values
  {
    std::vector<double> identicalArray = std::vector<double>(10, 3.);
    const int index = SHA_Collections::BinarySearch<Const_Double_Iterator_Type, double>(identicalArray.begin(), identicalArray.end(), static_cast<const double>(3.));
    EXPECT_EQ(5, index);
  }
}


// Test SimpleStockMarketProblem on a simple market array
TEST(TestBasicSearch, MaxDistanceBasics)
{
  // Should return <4,9> (largest benefice of 6)
  {
    const std::vector<int> marketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> maxBeneficeIndexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int> >(marketPrices.begin(), marketPrices.end());
    EXPECT_EQ(4, maxBeneficeIndexes.first);
    EXPECT_EQ(9, maxBeneficeIndexes.second);
  }

  // Should return <0, idxEnd> on sorted array
  {
    const std::vector<int> sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> indexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int> >(sortedArray.begin(), sortedArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(static_cast<int>(sortedArray.size()) - 1, indexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const std::vector<int> insufficientArray = std::vector<int>(1, 2);
    const std::pair<int, int> indexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int> >(insufficientArray.begin(), insufficientArray.end());
    EXPECT_EQ(-1, indexes.first);
    EXPECT_EQ(-1, indexes.second);
  }

  // Should return <0,1> on array containing only two elements
  {
    const std::vector<int> twoElementArray = std::vector<int>(2, 2);
    const std::pair<int, int> indexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int> >(twoElementArray.begin(), twoElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // Should return <0,1> on array containing the same value
  {
    const std::vector<int> sameElementArray = std::vector<int>(10, 2);
    const std::pair<int, int> indexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int> >(sameElementArray.begin(), sameElementArray.end());
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
    const std::pair<int, int> kMaxBeneficeIndexes = SHA_Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(kMarketPrices);
    EXPECT_EQ(5, kMaxBeneficeIndexes.first);
    EXPECT_EQ(9, kMaxBeneficeIndexes.second);
  }

  // Should return <FirstPositiveIdx, idxEnd> on sorted array
  {
    const std::vector<int> kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> kIndexes = SHA_Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(kSortedArray);
    EXPECT_EQ(2, kIndexes.first);
    EXPECT_EQ(static_cast<int>(kSortedArray.size()) - 1, kIndexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const std::pair<int, int> kIndexes = SHA_Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(1, 2));
    EXPECT_EQ(-1, kIndexes.first);
    EXPECT_EQ(-1, kIndexes.second);
  }

  // Should return <0,1> on array containing only two positive elements
  {
    const std::pair<int, int> kIndexes = SHA_Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(2, 2));
    EXPECT_EQ(0, kIndexes.first);
    EXPECT_EQ(1, kIndexes.second);
  }

  // Should return <0, idxEnd> on array containing the same positive value
  {
    const int kSize = 10;
    const std::pair<int, int> indexes = SHA_Collections::MaxSubArray<int, std::minus<int>, std::greater<int> >(std::vector<int>(kSize, 2));
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
    const std::vector<int> kMMaxElements = SHA_Collections::MaxMElements<int, std::greater_equal<int> >(kRandomElements, 1);
    EXPECT_EQ(5, kMMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    const std::vector<int> kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = SHA_Collections::MaxMElements<int, std::greater_equal<int> >(kRandomElements, 3);
    EXPECT_EQ(5, kMMaxElements[0]);
    EXPECT_EQ(5, kMMaxElements[1]);
    EXPECT_EQ(4, kMMaxElements[2]);
  }

  // Should return the last elements on sorted vector
  {
    const std::vector<int> kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = SHA_Collections::MaxMElements<int, std::greater_equal<int> >(kSortedArray, 4);
    EXPECT_EQ(366, kMMaxElements[0]);
    EXPECT_EQ(212, kMMaxElements[1]);
    EXPECT_EQ(36, kMMaxElements[2]);
    EXPECT_EQ(15, kMMaxElements[3]);
  }

  // Should return empty vector on insufficient vector
  {
    const std::vector<int> kMMaxElements = SHA_Collections::MaxMElements<int, std::greater_equal<int> >(std::vector<int>(1, 2), 2);
    EXPECT_EQ(0, static_cast<int>(kMMaxElements.size()));
  }

  // Should return empty vector when looking for less than 1 elements
  {
    const std::vector<int> kMMaxElements = SHA_Collections::MaxMElements<int, std::greater_equal<int> >(std::vector<int>(1, 2), 0);
    EXPECT_EQ(0, static_cast<int>(kMMaxElements.size()));
  }
}

// Test MaxNElements to find the lowest values
TEST(TestBasicSearch, MaxNElementsLowestValues)
{
  // Should return the min value [-18] for a unique element search
  {
    const std::vector<int> kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = SHA_Collections::MaxMElements<int, std::less_equal<int> >(kRandomElements, 1);
    EXPECT_EQ(-18, kMMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    const std::vector<int> kRandomElements(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = SHA_Collections::MaxMElements<int, std::less_equal<int> >(kRandomElements, 3);
    EXPECT_EQ(-18, kMMaxElements[0]);
    EXPECT_EQ(-5, kMMaxElements[1]);
    EXPECT_EQ(2, kMMaxElements[2]);
  }

  // Should return the first elements on sorted vector
  {
    const std::vector<int> kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::vector<int> kMMaxElements = SHA_Collections::MaxMElements<int, std::less_equal<int> >(kSortedArray, 4);
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
    Iterator_type it = SHA_Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.end(), 4);
    EXPECT_EQ(ksortedArray.begin() + 4, it);
  }

  // Basic run on random array - Should return 4
  {
    std::vector<int> krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_type::value_type value = *SHA_Collections::KthMaxElement<Iterator_type, Comparator_type>(krandomdArray.begin(), krandomdArray.end(), 7);
    EXPECT_EQ(4, value);
  }

  // Basic run with extreme values
  {
    std::vector<int> ksortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_type it;

    it = SHA_Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.begin(), 0);
    EXPECT_EQ(ksortedArray.begin(), it);     // Should return end on empty sequence
    it = SHA_Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.begin() + 1, 0);
    EXPECT_EQ(ksortedArray.begin(), it);     // Should return the unique element
    it = SHA_Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.end(), -1);
    EXPECT_EQ(ksortedArray.end(), it);       // Should end for out of scope k (-1)
    it =  SHA_Collections::KthMaxElement<Iterator_type, Comparator_type>(ksortedArray.begin(), ksortedArray.end(), 100);
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
    Iterator_type::value_type value = *SHA_Collections::KthMaxElement<Iterator_type, Comparator_type>(krandomdArray.begin(), krandomdArray.end(), 1);
    EXPECT_EQ(5, value);
  }
}

// Test intersections
TEST(TestBasicSearch, BasicIntersections)
{
  typedef std::vector<int> Vector_type;

  EXPECT_EQ(0, SHA_Collections::Intersection(Vector_type(), Vector_type()).size());    // Null intersection on empty vectors
  EXPECT_EQ(0, SHA_Collections::Intersection(Vector_type(1,1), Vector_type()).size()); // Null intersection with empty vector

  // Basic run with same vector
  {
    const Vector_type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

    Vector_type intersectionVector = SHA_Collections::Intersection<int>(sortedArray, sortedArray);
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

    Vector_type intersectionVector = SHA_Collections::Intersection<int>(firstRandomVector, secondRandomVector);
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

    Vector_type intersectionVector = SHA_Collections::Intersection<int>(firstRandomVector, secondRandomVector);

    // Should return [-18, -5, 3, 5, 5] after sorting
    std::sort(intersectionVector.begin(), intersectionVector.end());
    EXPECT_EQ(-18, intersectionVector[0]);
    EXPECT_EQ(-5, intersectionVector[1]);
    EXPECT_EQ(3, intersectionVector[2]);
    EXPECT_EQ(5, intersectionVector[3]);
    EXPECT_EQ(5, intersectionVector[4]);
  }
}
