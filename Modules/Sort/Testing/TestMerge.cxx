/*===========================================================================================================
 *
 * SHA - Simple Hybesis Algorithms
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#include <gtest/gtest.h>
#include <merge.hxx>

// STD includes
#include <functional>
#include <vector>
#include <string>

// Testing namespace
using namespace SHA_Sort;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};       // Simple sorted array of integers with negative values
  const int SortedArrayIntWithRot[] = {-3, 2, 7, 20, 0, 2, 8, 15, 36};    // Simple sorted array of integers with negative values
  const int SortedArrayIntPos[] = {0, 2, 8, 15, 36, 212, 366, 15478};     // Simple sorted array of integers with positive values only
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};      // Simple random array of integers with negative values
  const int RandomArrayIntPos[] = {4520, 30, 500, 20, 3, 2, 3, 4, 5, 15}; // Simple random array of integers with positive values only
  const std::string RandomStr = "xacvgeze";                               // Random string
  const std::string StringWithPivot = "eknxasuw";                         // String with pivot at end =  begin() + 4 : Left sorted part [e,k,n,x] - Right sorted part [a,s,u,w]

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
  typedef std::less_equal<IT::value_type> Comparator_type;
  typedef MergeWithBuffer<Container, IT> Aggregator;
  typedef MergeWithBuffer<std::string, std::string::iterator> AggregatorStr_type;
}
#endif /* DOXYGEN_SKIP */

// Basic MergeInPlace tests
TEST(TestMerge, MergeInPlaces)
{
  // Normal Run - All elements should be sorted in order
  {
    Container sortedArrayWithRot
      (SortedArrayIntWithRot, SortedArrayIntWithRot + sizeof(SortedArrayIntWithRot) / sizeof(int));
    MergeInPlace<IT>()
      (sortedArrayWithRot.begin(), sortedArrayWithRot.begin() + 4, sortedArrayWithRot.end());

    // All elements of the final array are sorted
    for (IT it = sortedArrayWithRot.begin(); it < sortedArrayWithRot.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container sortedArrayPos
      (SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    MergeInPlace<IT>()(sortedArrayPos.begin(), sortedArrayPos.begin() + 5, sortedArrayPos.end());

    // All elements are still sorted
    for (IT it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container randomArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    MergeInPlace<IT>()(randomArrayPos.end(), randomArrayPos.begin() + 3, randomArrayPos.begin());

    int i = 0;
    for (IT it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container emptyArray;
    MergeInPlace<IT>()(emptyArray.begin(), emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    MergeInPlace<IT>()(uniqueValueArray.begin(), uniqueValueArray.end(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // Double values array - Order should be made
  {
    Container doubleValuesArray(1, 511);
    doubleValuesArray.push_back(66);

    MergeInPlace<IT>()
      (doubleValuesArray.begin(), doubleValuesArray.begin() + 1, doubleValuesArray.end());

    EXPECT_EQ(66, doubleValuesArray[0]);
    EXPECT_EQ(511, doubleValuesArray[1]);
  }

  // String Collection - All elements should be sorted in order
  {
    std::string stringToMerge = StringWithPivot;
    MergeInPlace<std::string::iterator>()
      (stringToMerge.begin(), stringToMerge.begin() + 4, stringToMerge.end());

    // All elements of the final array are sorted
    for (std::string::iterator it = stringToMerge.begin(); it < stringToMerge.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}


// Basic MergeWithBuffer tests
TEST(TestMerge, MergeWithBuffers)
{
  // Normal Run - All elements should be sorted in order
  {
    Container sortedArrayWithRot
      (SortedArrayIntWithRot, SortedArrayIntWithRot + sizeof(SortedArrayIntWithRot) / sizeof(int));
    MergeWithBuffer<Container, IT>()
      (sortedArrayWithRot.begin(), sortedArrayWithRot.begin() + 4, sortedArrayWithRot.end());

    // All elements of the final array are sorted
    for (IT it = sortedArrayWithRot.begin(); it < sortedArrayWithRot.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container sortedArrayPos
      (SortedArrayIntPos, SortedArrayIntPos + sizeof(SortedArrayIntPos) / sizeof(int));
    MergeWithBuffer<Container, IT>()
      (sortedArrayPos.begin(), sortedArrayPos.begin() + 5, sortedArrayPos.end());

    // All elements are still sorted
    for (IT it = sortedArrayPos.begin(); it < sortedArrayPos.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container randomArrayPos
      (RandomArrayIntPos, RandomArrayIntPos + sizeof(RandomArrayIntPos) / sizeof(int));
    MergeWithBuffer<Container, IT>()
      (randomArrayPos.end(), randomArrayPos.begin() + 3, randomArrayPos.begin());

    int i = 0;
    for (IT it = randomArrayPos.begin(); it < randomArrayPos.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayIntPos[i], *it);
  }

  // No error empty array
  {
    Container emptyArray;
    MergeWithBuffer<Container, IT>()
      (emptyArray.begin(), emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    MergeWithBuffer<Container, IT>()
      (uniqueValueArray.begin(), uniqueValueArray.end(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // Double values array - Order should be made
  {
    Container doubleValuesArray(1, 511);
    doubleValuesArray.push_back(66);

    MergeWithBuffer<Container, IT>()
      (doubleValuesArray.begin(), doubleValuesArray.begin() + 1, doubleValuesArray.end());

    EXPECT_EQ(66, doubleValuesArray[0]);
    EXPECT_EQ(511, doubleValuesArray[1]);
  }

  // String Collection - All elements should be sorted in order
  {
    std::string stringToMerge = "eknxasuw"; // Left sorted part [e,k,n,x] - Right sorted part [a,s,u,w]
    MergeWithBuffer<std::string, std::string::iterator>()
      (stringToMerge.begin(), stringToMerge.begin() + 4, stringToMerge.end());

    // All elements of the final array are sorted
    for (std::string::iterator it = stringToMerge.begin(); it < stringToMerge.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Basic Merge-Sort tests - Uses the merge with buffer (computation optimized)
TEST(TestMerge, MergeSorts)
{
  // Normal Run - all elements should be sorter in order
  {
    Container randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    MergeSort<Container, IT, Aggregator>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted
    for (IT it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    MergeSort<Container, IT, Aggregator>(sortedArray.begin(), sortedArray.end());

    // All elements are still sorted
    for (IT it = sortedArray.begin(); it < sortedArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    MergeSort<Container, IT, Aggregator>(randomdArray.end(), randomdArray.begin());

    int i = 0;
    for (IT it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }

  // No error empty array
  {
    Container emptyArray;
    MergeSort<Container, IT, Aggregator>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    MergeSort<Container, IT, Aggregator>
      (uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // String collection - all elements should be sorter in order
  {
    std::string randomStr = RandomStr;
    MergeSort<std::string, std::string::iterator, AggregatorStr_type>
      (randomStr.begin(), randomStr.end());

    // All elements are sorted
    for (std::string::iterator it = randomStr.begin(); it < randomStr.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}
