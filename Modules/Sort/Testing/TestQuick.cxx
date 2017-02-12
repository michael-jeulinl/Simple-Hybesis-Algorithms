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
#include <quick.hxx>

// STD includes
#include <functional>
#include <vector>
#include <string>

// Testing namespace
using namespace SHA_Sort;

#ifndef DOXYGEN_SKIP
namespace {
  const int SortedArrayInt[] = {-3, -2, 0, 2, 8, 15, 36, 212, 366};       // Simple sorted array of integers with negative values
  const int RandomArrayInt[] = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};      // Simple random array of integers with negative values
  const std::string RandomStr = "xacvgeze";                               // Random string

  typedef std::vector<int> Container;
  typedef Container::iterator IT;
  typedef std::greater_equal<IT::value_type> GE_Comparator;
}
#endif /* DOXYGEN_SKIP */

// Basic Quick-Sort tests
TEST(TestSort, QuickSorts)
{
  // Normal Run
  {
    Container randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    QuickSort<IT>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted
    for (IT it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already sortedArray - Array should not be affected
  {
    Container sortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    QuickSort<IT>(sortedArray.begin(), sortedArray.end());

    // All elements are still sorted
    for (IT it = sortedArray.begin(); it < sortedArray.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    QuickSort<IT>(randomdArray.end(), randomdArray.begin());

    int i = 0;
    for (IT it = randomdArray.begin(); it < randomdArray.end(); ++it, ++i)
      EXPECT_EQ(RandomArrayInt[i], *it);
  }

  // No error unitialized array
  {
    Container emptyArray;
    QuickSort<IT>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    QuickSort<IT>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // String - String should be sorted as an array
  {
    std::string stringToSort = RandomStr;
    QuickSort<std::string::iterator, std::less_equal<char>>(stringToSort.begin(), stringToSort.end());
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
    Container randomdArray(RandomArrayInt, RandomArrayInt + sizeof(RandomArrayInt) / sizeof(int));
    QuickSort<IT, GE_Comparator>(randomdArray.begin(), randomdArray.end());

    // All elements are sorted in inverse order
    for (IT it = randomdArray.begin(); it < randomdArray.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // Already sorted Array in inverse order - Array should not be affected
  {
    Container invSortedArray(SortedArrayInt, SortedArrayInt + sizeof(SortedArrayInt) / sizeof(int));
    QuickSort<IT, GE_Comparator>(invSortedArray.begin(), invSortedArray.end());

    // All elements are still sorted in inverse order
    for (IT it = invSortedArray.begin(); it < invSortedArray.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // String - String should be sorted in inverse order
  {
    std::string stringToSort = RandomStr;
    QuickSort<std::string::iterator, std::greater_equal<char>>(stringToSort.begin(), stringToSort.end());

    // All elements are sorted in inverse order
    for (std::string::iterator it = stringToSort.begin(); it < stringToSort.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }
}
