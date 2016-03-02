#include <gtest/gtest.h>
#include <search.hxx>

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const int RandomArrayInterInt[] = {5 , 5, -5, 3, -18, 10, 15};     // Other  random array of integers with negative values
  const double SortedDoubleArray[] = {-.3, 0.0, 0.12, 2.5, 8};       // Simple sorted array of floats with negative values
  const std::string OrderedStr = "acegmnoop";                        // Ordered string
  const std::string RandomStr = "xacvgeze";                          // Random string

  typedef std::vector<int> Container_Type;
  typedef Container_Type::iterator Iterator_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
  typedef std::vector<double>::const_iterator Const_Double_Iterator_Type;
  typedef std::less_equal<Container_Type::value_type> Comparator_type;
  typedef std::greater_equal<Container_Type::value_type> Greater_Comparator_type;

  template <typename T>
  struct EQUIVALENT
  {
    bool operator()(const T& a, const T& b) const
    { return std::abs(a - b) < std::numeric_limits<T>::epsilon(); }
  };

  template <typename T>
  struct EQUAL
  {
    bool operator()(const T& a, const T& b) const { return a == b; }
  };
}

// Basic BinarySearchIterative tests on a sorted array of ints
TEST(TestSearch, BinarySearchBasics)
{
  const Container_Type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));

  // Empty array
  {
    Container_Type emptyArray = Container_Type();
    const int index = SHA_Collections::BinarySearch<Const_Iterator_Type, int, EQUAL<int>>
      (emptyArray.begin(), emptyArray.end(), 0);
    EXPECT_EQ(-1, index); // Should return -1 on empty array
  }

  // First element
  {
    const int index = SHA_Collections::BinarySearch<Const_Iterator_Type, int, EQUAL<int>>
      (sortedArray.begin(), sortedArray.end(), -3);
    EXPECT_EQ(0, index);
  }

  // Existing random value
  {
    const int index = SHA_Collections::BinarySearch<Const_Iterator_Type, int, EQUAL<int>>
      (sortedArray.begin(), sortedArray.end(), 8);
    EXPECT_EQ(4, index);
  }

  // Non-existing element
  {
    const int index = SHA_Collections::BinarySearch<Const_Iterator_Type, int, EQUAL<int>>
      (sortedArray.begin(), sortedArray.end(), 1);
    EXPECT_EQ(-1, index);
  }

  // String collection - Find letter
  {
    const int index = SHA_Collections::BinarySearch<std::string::const_iterator, char, EQUAL<char>>
      (OrderedStr.begin(), OrderedStr.end(), 'm');
    EXPECT_EQ(4, index);
  }
}

// Basic BinarySearchIterative tests on a sorted array of doubles
TEST(TestSearch, BinarySearchDoubles)
{
  const std::vector<double> sortedDoubleArray
    (SortedDoubleArray, SortedDoubleArray + sizeof(SortedDoubleArray) / sizeof(double));

  // First element
  {
    const int index = SHA_Collections::BinarySearch<Const_Double_Iterator_Type, double, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(-.3));
    EXPECT_EQ(0, index);
  }

  // Existing element
  {
    const int index = SHA_Collections::BinarySearch<Const_Double_Iterator_Type, double, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(0.12));
    EXPECT_EQ(2, index);
  }

  // Non Existing element
  {
    const int index = SHA_Collections::BinarySearch<Const_Double_Iterator_Type, double, EQUIVALENT<double>>
      (sortedDoubleArray.begin(), sortedDoubleArray.end(), static_cast<const double>(8.1));
    EXPECT_EQ(-1, index);
  }

  // Value in the middle when identical values
  {
    const std::vector<double> identicalArray = std::vector<double>(10, 3.);
    const int index = SHA_Collections::BinarySearch<Const_Double_Iterator_Type, double, EQUIVALENT<double>>
      (identicalArray.begin(), identicalArray.end(), static_cast<const double>(3.));
    EXPECT_EQ(5, index);
  }
}


// Test SimpleStockMarketProblem on a simple market array
TEST(TestSearch, MaxDistances)
{
  // Should return <4,9> (largest benefice of 6)
  {
    const Container_Type marketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> maxBeneficeIndexes =
      SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int>>
      (marketPrices.begin(), marketPrices.end());
    EXPECT_EQ(4, maxBeneficeIndexes.first);
    EXPECT_EQ(9, maxBeneficeIndexes.second);
  }

  // Should return <0, idxEnd> on sorted array
  {
    const Container_Type sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> indexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int>>
      (sortedArray.begin(), sortedArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(static_cast<int>(sortedArray.size()) - 1, indexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const Container_Type insufficientArray = Container_Type(1, 2);
    const std::pair<int, int> indexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int>>
      (insufficientArray.begin(), insufficientArray.end());
    EXPECT_EQ(-1, indexes.first);
    EXPECT_EQ(-1, indexes.second);
  }

  // Should return <0,1> on array containing only two elements
  {
    const Container_Type twoElementArray = Container_Type(2, 2);
    const std::pair<int, int> indexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int>>
      (twoElementArray.begin(), twoElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // Should return <0,1> on array containing the same value
  {
    const Container_Type sameElementArray = Container_Type(10, 2);
    const std::pair<int, int> indexes = SHA_Collections::MaxDistance<Const_Iterator_Type, std::minus<int>>
      (sameElementArray.begin(), sameElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(1, indexes.second);
  }

  // String - should return <1,0> as 'a', 'z' are the most distanced letter
  {
    const std::pair<int, int> indexes =
      SHA_Collections::MaxDistance<std::string::const_iterator, std::minus<char>>
      (RandomStr.begin(), RandomStr.end());
    EXPECT_EQ(1, indexes.first);
    EXPECT_EQ(6, indexes.second);
  }
}


// Test MaxSubArray
TEST(TestSearch, MaxSubArrays)
{
  // Should return <5,9> (maximal sum of 17)
  {
    const Container_Type kMarketPrices(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const std::pair<int, int> kMaxBeneficeIndexes =
      SHA_Collections::MaxSubArray<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (kMarketPrices.begin(), kMarketPrices.end());
    EXPECT_EQ(5, kMaxBeneficeIndexes.first);
    EXPECT_EQ(9, kMaxBeneficeIndexes.second);
  }

  // Should return <FirstPositiveIdx, idxEnd> on sorted array
  {
    const Container_Type kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const std::pair<int, int> kIndexes =
      SHA_Collections::MaxSubArray<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (kSortedArray.begin(), kSortedArray.end());
    EXPECT_EQ(2, kIndexes.first);
    EXPECT_EQ(static_cast<int>(kSortedArray.size()) - 1, kIndexes.second);
  }

  // Should return <-1,-1> on insufficient array
  {
    const Container_Type insufficientArray = Container_Type(1, 2);
    const std::pair<int, int> kIndexes =
      SHA_Collections::MaxSubArray<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (insufficientArray.begin(), insufficientArray.end());
    EXPECT_EQ(-1, kIndexes.first);
    EXPECT_EQ(-1, kIndexes.second);
  }

  // Should return <0,1> on array containing only two positive elements
  {
    const Container_Type twoElementArray = Container_Type(2, 2);
    const std::pair<int, int> kIndexes =
      SHA_Collections::MaxSubArray<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (twoElementArray.begin(), twoElementArray.end());
    EXPECT_EQ(0, kIndexes.first);
    EXPECT_EQ(1, kIndexes.second);
  }

  // Should return <0, idxEnd> on array containing the same positive value
  {
    const int kSize = 10;
    Container_Type sameElementArray = Container_Type(kSize, 2);
    const std::pair<int, int> indexes =
      SHA_Collections::MaxSubArray<Const_Iterator_Type, std::minus<int>, std::greater<int>>
      (sameElementArray.begin(), sameElementArray.end());
    EXPECT_EQ(0, indexes.first);
    EXPECT_EQ(kSize - 1, indexes.second);
  }
}

// Test MaxNElements
TEST(TestSearch, MaxNElementss)
{
  // Should return the max value [5] for a unique element search
  {
    const Container_Type kRandomElements
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Container_Type kMMaxElements =
      SHA_Collections::MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 1);
    EXPECT_EQ(5, kMMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    const Container_Type kRandomElements
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Container_Type kMMaxElements =
      SHA_Collections::MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 3);
    EXPECT_EQ(5, kMMaxElements[0]);
    EXPECT_EQ(5, kMMaxElements[1]);
    EXPECT_EQ(4, kMMaxElements[2]);
  }

  // Should return the last elements on sorted vector
  {
    const Container_Type kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Container_Type kMMaxElements =
      SHA_Collections::MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (kSortedArray.begin(), kSortedArray.end(), 4);
    EXPECT_EQ(366, kMMaxElements[0]);
    EXPECT_EQ(212, kMMaxElements[1]);
    EXPECT_EQ(36, kMMaxElements[2]);
    EXPECT_EQ(15, kMMaxElements[3]);
  }

  // Should return empty vector on insufficient vector
  {
    const Container_Type oneElementCollection = Container_Type(1, 2);
    const Container_Type kMMaxElements =
      SHA_Collections::MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (oneElementCollection.begin(), oneElementCollection.end(), 2);
    EXPECT_EQ(0, static_cast<int>(kMMaxElements.size()));
  }

  // Should return empty vector when looking for less than 1 elements
  {
    const Container_Type oneElementCollection = Container_Type(1, 2);
    const Container_Type kMMaxElements =
      SHA_Collections::MaxMElements<Container_Type, Const_Iterator_Type, std::greater_equal<int>>
      (oneElementCollection.begin(), oneElementCollection.end(), 0);
    EXPECT_EQ(0, static_cast<int>(kMMaxElements.size()));
  }
}

// Test MaxNElements to find the lowest values
TEST(TestSearch, MaxNElementsLowestValues)
{
  // Should return the min value [-18] for a unique element search
  {
    const Container_Type kRandomElements
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const Container_Type kMMaxElements =
      SHA_Collections::MaxMElements<Container_Type, Const_Iterator_Type, std::less_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 1);
    EXPECT_EQ(-18, kMMaxElements[0]);
  }

  // Should return [5,5,4]
  {
    const Container_Type kRandomElements
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    const Container_Type kMMaxElements =
      SHA_Collections::MaxMElements<Container_Type, Const_Iterator_Type, std::less_equal<int>>
      (kRandomElements.begin(), kRandomElements.end(), 3);
    EXPECT_EQ(-18, kMMaxElements[0]);
    EXPECT_EQ(-5, kMMaxElements[1]);
    EXPECT_EQ(2, kMMaxElements[2]);
  }

  // Should return the first elements on sorted vector
  {
    const Container_Type kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    const Container_Type kMMaxElements =
      SHA_Collections::MaxMElements<Container_Type, Const_Iterator_Type, std::less_equal<int>>
      (kSortedArray.begin(), kSortedArray.end(), 4);
    EXPECT_EQ(-3, kMMaxElements[0]);
    EXPECT_EQ(-2, kMMaxElements[1]);
    EXPECT_EQ(0, kMMaxElements[2]);
    EXPECT_EQ(2, kMMaxElements[3]);
  }
}

// Test kth smallest elements
TEST(TestSearch, KthSmallests)
{
  // Basic run on sorted array with unique element - Should the kth element
  {
    Container_Type ksortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_Type it = SHA_Collections::KthMaxElement<Iterator_Type, Comparator_type>
      (ksortedArray.begin(), ksortedArray.end(), 4);
    EXPECT_EQ(ksortedArray.begin() + 4, it);
  }

  // Basic run on random array - Should return 4
  {
    Container_Type krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_Type::value_type value = *SHA_Collections::KthMaxElement<Iterator_Type, Comparator_type>
      (krandomdArray.begin(), krandomdArray.end(), 7);
    EXPECT_EQ(4, value);
  }

  // Basic run with extreme values
  {
    Container_Type ksortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    Iterator_Type it;

    it = SHA_Collections::KthMaxElement<Iterator_Type, Comparator_type>
      (ksortedArray.begin(), ksortedArray.begin(), 0);
    EXPECT_EQ(ksortedArray.begin(), it);     // Should return end on empty sequence
    it = SHA_Collections::KthMaxElement<Iterator_Type, Comparator_type>
      (ksortedArray.begin(), ksortedArray.begin() + 1, 0);
    EXPECT_EQ(ksortedArray.begin(), it);     // Should return the unique element
    it = SHA_Collections::KthMaxElement<Iterator_Type, Comparator_type>
      (ksortedArray.begin(), ksortedArray.end(), -1);
    EXPECT_EQ(ksortedArray.end(), it);       // Should end for out of scope k (-1)
    it =  SHA_Collections::KthMaxElement<Iterator_Type, Comparator_type>
      (ksortedArray.begin(), ksortedArray.end(), 100);
    EXPECT_EQ(ksortedArray.end(), it);       // Should end for out of scope k (100)
  }

  // String
  {
    std::string randomStr = RandomStr;
    const char secondSmallestLetter = *SHA_Collections::KthMaxElement
      <std::string::iterator, std::less_equal<char>>(randomStr.begin(), randomStr.end(), 1);
    EXPECT_EQ('c', secondSmallestLetter);
  }
}

// Test kth biggest element
TEST(TestSearch, KthBiggests)
{
  // Basic run on random array - Should return 5 (second biggest value)
  {
    Container_Type krandomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    Iterator_Type::value_type value = *SHA_Collections::KthMaxElement<Iterator_Type, Greater_Comparator_type>
      (krandomdArray.begin(), krandomdArray.end(), 1);
    EXPECT_EQ(5, value);
  }
}
