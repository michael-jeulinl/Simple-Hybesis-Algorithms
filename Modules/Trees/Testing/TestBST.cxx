#include <gtest/gtest.h>
#include <BST.hxx>

#include <list>

using namespace SHA_Trees;

namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // Simple random array of integers with negative values
  const int RandomArrayInterInt[] = {5 , 5, -5, 3, -18, 10, 15};     // Other  random array of integers with negative values
  const int SmallIntArray[] = {2, 1, 3};                             // Small array containing 2, 1, 3 values
  const int SmallIntArraySorted[] = {1, 2, 3};                       // Small array ordered containing 1, 2, 3 values
  const std::string OrderedStr = "acegmnoop";                        // Ordered string
  const std::string RandomStr = "xacvgeze";                          // Random string

  typedef std::vector<int> Container_Type;
  typedef Container_Type::value_type Value_type;
  typedef Container_Type::iterator Iterator_Type;
  typedef Container_Type::const_iterator Const_Iterator_Type;
  typedef BST<Const_Iterator_Type> Const_BST_type;
  typedef std::unique_ptr<Const_BST_type> Const_BST_Owner_Type;
}

// Test BST Construction
TEST(TestBST, build)
{
  // Empty Array - No BST should be built
  {
    const Container_Type kEmptyCollection = Container_Type();
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_FALSE(tree);
  }

  // Begin After End - No BST should be built
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.end(), kSmallIntArray.begin());
    EXPECT_FALSE(tree);
  }

  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(2, tree->GetData());
    EXPECT_TRUE(tree->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild() == nullptr);
  }

  // Basic construction
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.end());

    EXPECT_EQ(2, tree->GetData());
    EXPECT_EQ(1, tree->GetLeftChild()->GetData());
    EXPECT_EQ(3, tree->GetRightChild()->GetData());
    EXPECT_TRUE(tree->GetLeftChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetLeftChild()->GetRightChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetRightChild() == nullptr);
  }
}

// Test BST Construction From Sorted sequence
TEST(TestBST, buildFromSorted)
{
  // Empty Array - No BST should be built
  {
    const Container_Type kEmptyCollection = Container_Type();
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kEmptyCollection.begin(), kEmptyCollection.end());
    EXPECT_FALSE(tree);
  }

  // Begin After End - No BST should be built
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.end(), kSmallIntArray.begin());
    EXPECT_FALSE(tree);
  }

  // Unique element - Root should be created
  {
    const Container_Type kSmallIntArray(SmallIntArray, SmallIntArray + sizeof(SmallIntArray) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::Build(kSmallIntArray.begin(), kSmallIntArray.begin() + 1);
    EXPECT_EQ(2, tree->GetData());
    EXPECT_TRUE(tree->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild() == nullptr);
  }

  // Basic construction on sorted array
  {
    const Container_Type kSmallSorted(SmallIntArraySorted, SmallIntArraySorted + sizeof(SmallIntArraySorted) / sizeof(Value_type));
    Const_BST_Owner_Type tree =  Const_BST_type::BuildFromSorted(kSmallSorted.begin(), kSmallSorted.end());

    EXPECT_EQ(2, tree->GetData());
    EXPECT_EQ(1, tree->GetLeftChild()->GetData());
    EXPECT_EQ(3, tree->GetRightChild()->GetData());
    EXPECT_TRUE(tree->GetLeftChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetLeftChild()->GetRightChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetLeftChild() == nullptr);
    EXPECT_TRUE(tree->GetRightChild()->GetRightChild() == nullptr);
  }
}
