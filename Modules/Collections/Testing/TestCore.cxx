#include <gtest/gtest.h>
#include <core.hxx>

#include <list>

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const int RandomArrayInterInt[] = {5 , 5, -5, 3, -18, 10, 15};     // Other  random array of integers with negative values
  const int SmallIntArray[] = {2, 1, 3};                             // Small array containing 2, 1, 3 values
  const std::string OrderedStr = "acegmnoop";                        // Ordered string
  const std::string RandomStr = "xacvgeze";                          // Random string

  typedef std::vector<int> Container_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
  typedef std::list<Container_Type> Output_Container_Type;
}

// Test intersections
TEST(TestCore, Intersections)
{
  // Null intersection on empty vectors - empty intersection expected
  {
    const Container_Type kEmptyCollection = Container_Type();
    Container_Type intersection = SHA_Collections::Intersection<Container_Type, Const_Iterator_Type>
      (kEmptyCollection.begin(), kEmptyCollection.end(), kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_EQ(0, intersection.size());
  }

  // Null intersection with one empty vector - empty intersection expected
  {
    const Container_Type kEmptyCollection = Container_Type();
    const Container_Type kCollection = Container_Type(10,1);
    Container_Type intersection = SHA_Collections::Intersection<Container_Type, Const_Iterator_Type>
      (kEmptyCollection.begin(), kEmptyCollection.end(), kCollection.begin(), kCollection.end());
    EXPECT_EQ(0, intersection.size());
  }

  // Basic run with same collection - intersection as the collection itself expected
  {
    const Container_Type kSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) /
      sizeof(Container_Type::value_type));
    Container_Type intersection = SHA_Collections::Intersection<Container_Type, Const_Iterator_Type>
      (kSortedArray.begin(), kSortedArray.end(), kSortedArray.begin(), kSortedArray.end());

    // The intersection of the same vector should return the same vector
    Const_Iterator_Type kIntersectIt = intersection.begin();
    for (Const_Iterator_Type it = kSortedArray.begin(); it != kSortedArray.end(); ++it, ++kIntersectIt)
      EXPECT_EQ(*it, *kIntersectIt);
  }

  // Basic run with same collection containing duplicates- intersection as the collection itself expected
  {
    const Container_Type kFirstRandom(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) /
      sizeof(Container_Type::value_type));
    const Container_Type kSecondRandom(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) /
      sizeof(Container_Type::value_type));

    Container_Type intersection = SHA_Collections::Intersection<Container_Type, Const_Iterator_Type>
      (kFirstRandom.begin(), kFirstRandom.end(), kSecondRandom.begin(), kSecondRandom.end());

    // The intersection with the copy vector should return the same vector
    Const_Iterator_Type kIntersectIt = intersection.begin();
    for (Const_Iterator_Type it = kFirstRandom.begin(); it != kFirstRandom.end(); ++it, ++kIntersectIt)
      EXPECT_EQ(*it, *kIntersectIt);
  }

  // Basic run with normal values
  {
    const Container_Type kFirstRandom
      (RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(Container_Type::value_type));
    const Container_Type kSecondRandom
      (RandomArrayInterInt, RandomArrayInterInt + sizeof(RandomArrayInterInt) /
      sizeof(Container_Type::value_type));

    Container_Type intersection = SHA_Collections::Intersection<Container_Type, Const_Iterator_Type>
      (kFirstRandom.begin(), kFirstRandom.end(), kSecondRandom.begin(), kSecondRandom.end());

    // Should return [-18, -5, 3, 5, 5] after sorting
    std::sort(intersection.begin(), intersection.end());
    EXPECT_EQ(-18, intersection[0]);
    EXPECT_EQ(-5, intersection[1]);
    EXPECT_EQ(3, intersection[2]);
    EXPECT_EQ(5, intersection[3]);
    EXPECT_EQ(5, intersection[4]);
  }

  // String run
  {
    std::string intersection = SHA_Collections::Intersection<std::string, std::string::const_iterator>
      (OrderedStr.begin(), OrderedStr.end(), RandomStr.begin(), RandomStr.end());

    // Should return ['a', 'c', 'e', 'g'] after sorting
    std::sort(intersection.begin(), intersection.end());
    EXPECT_EQ('a', intersection[0]);
    EXPECT_EQ('c', intersection[1]);
    EXPECT_EQ('e', intersection[2]);
    EXPECT_EQ('g', intersection[3]);
  }
}

// Test permutations
TEST(TestCore, Permutations)
{

  // Empty vector - no permutations
  {
    const Container_Type kEmptyCollection = Container_Type();
    Output_Container_Type permutations = SHA_Collections::Permutations<Container_Type, Const_Iterator_Type>
      (kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_EQ(0, permutations.size());
  }

  // Inversed iterator - no permutations
  {
    const Container_Type kUnicCollection = Container_Type(1,10);
    Output_Container_Type permutations = SHA_Collections::Permutations<Container_Type, Const_Iterator_Type>
      (kUnicCollection.end(), kUnicCollection.begin());
    EXPECT_EQ(0, permutations.size());
  }

  // Unic element vector - Unique object returned as permutation
  {
    const Container_Type kUnicCollection = Container_Type(1,10);
    Output_Container_Type permutations = SHA_Collections::Permutations<Container_Type, Const_Iterator_Type>
      (kUnicCollection.begin(), kUnicCollection.end());
    EXPECT_EQ(1, permutations.size());
    EXPECT_EQ(1, permutations.begin()->size());
    EXPECT_EQ(10, *(permutations.begin()->begin()));
  }

  // Same elements vector - n! list of n equivalent elements
  {
    const Container_Type kSameElCollection = Container_Type(3,10);
    Output_Container_Type permutations = SHA_Collections::Permutations<Container_Type, Const_Iterator_Type>
      (kSameElCollection.begin(), kSameElCollection.end());
    EXPECT_EQ(6, permutations.size());
    for (Output_Container_Type::const_iterator it = permutations.begin(); it != permutations.end(); ++it)
    {
      EXPECT_EQ(3, it->size());
      for (Container_Type::const_iterator itEl = it->begin(); itEl != it->end(); ++itEl)
        EXPECT_EQ(10, *itEl);
    }
  }

  // Run with value 2, 1, 3
  {
    const Container_Type kSmallArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) /
      sizeof(Container_Type::value_type));
    Output_Container_Type permutations = SHA_Collections::Permutations<Container_Type, Const_Iterator_Type>
      (kSmallArray.begin(), kSmallArray.end());
    EXPECT_EQ(6, permutations.size());
    for (Output_Container_Type::const_iterator it = permutations.begin(); it != permutations.end(); ++it)
      EXPECT_EQ(3, it->size());
    //@TODO check sequence by sequence? (non ordered)
  }

  // String run
  {
    const std::string abcStr = "abc";
    std::list<std::string> permutations =
      SHA_Collections::Permutations<std::string, std::string::const_iterator>(abcStr.begin(), abcStr.end());
    EXPECT_EQ(6, permutations.size());
    for (std::list<std::string>::const_iterator it = permutations.begin(); it != permutations.end(); ++it)
      EXPECT_EQ(3, it->size());
    //@TODO check sequence by sequence? (non ordered)
  }
}
