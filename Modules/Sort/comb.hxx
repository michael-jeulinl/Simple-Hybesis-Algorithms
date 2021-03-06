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
#ifndef MODULE_SORT_COMB_HXX
#define MODULE_SORT_COMB_HXX

// STD includes
#include <iterator>

namespace SHA_Sort
{
  /// Comb Sort - Proceed an in-place sort on the elements.
  ///
  /// @tparam IT type using to go through the collection.
  /// @tparam Compare functor type (std::less_equal in order, std::greater_equal for inverse order).
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename IT,
            typename Compare = std::less<typename std::iterator_traits<IT>::value_type>>
  void Comb(const IT& begin, const IT& end)
  {
    const auto distance = static_cast<const int>(std::distance(begin, end));
    if (distance < 2)
      return;

    auto gap = distance;
    double shrink = 1.3;
    bool hasSwapped = true;
    while (hasSwapped)
    {
      hasSwapped = false;

      gap /= shrink;
      if (gap > 1)
        hasSwapped = true;
      else
        gap = 1;

      for (auto it = begin; it + gap < end; ++it)
        if (Compare()(*(it + gap), *it))
        {
          std::swap(*it, *(it + gap));
          hasSwapped = true;
        }
    }
  }
}

#endif // MODULE_SORT_COMB_HXX
