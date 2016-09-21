#ifndef MODULE_COMBINATORY_INTERSECTION_HXX
#define MODULE_COMBINATORY_INTERSECTION_HXX

// STD includes
#include <set>

namespace SHA_Combinatory
{
  /// Intersection - Return Intersection of the two sequences.
  ///
  /// @remark Retrieve the intersection of two sequences keeping dupplicate keys distinct.
  ///
  /// @complexity O(N + M).
  ///
  /// @tparam Iterator type using to go through the collection.
  ///
  /// @param beginFirst,endFirst,beginSecond,endSecond - iterators to the initial and final positions of
  /// the sequences. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return a vector containing the intersection of both sequences.
  template <typename Container, typename Iterator>
  Container Intersection(const Iterator& beginFirst, const Iterator& endFirst,
                         const Iterator& beginSecond, const Iterator& endSecond)
  {
    // Take the smallest sequence for initial count
    const size_t kFirstSize = std::distance(beginFirst, endFirst);
    const size_t kSecondSize = std::distance(beginSecond, endSecond);
    const bool kIsFirstSmaller = (kFirstSize <= kSecondSize);

    // Create and set enough capacity for the intersection
    Container intersection;
    intersection.reserve((kIsFirstSmaller) ? kFirstSize : kSecondSize);

    // Count each element of the smaller array
    std::multiset<Iterator::value_type> count;
    const Iterator kCountEndIt = (kIsFirstSmaller) ? endFirst : endSecond;
    for (Container::const_iterator it = (kIsFirstSmaller) ? beginFirst : beginSecond;
        it != kCountEndIt; ++it)
        count.insert(*it);

    // Push the element if find into the multiset and delete its instance from the counter
    const Iterator kIntersectEndIt = (kIsFirstSmaller) ? endSecond : endFirst;
    for (Container::const_iterator it = (kIsFirstSmaller) ? beginSecond : beginFirst;
         it != kIntersectEndIt; ++it)
    {
      std::multiset<Container::value_type>::iterator foundIt = count.find(*it);

      // Move element from count to intersection if found
      if (foundIt != count.end())
      {
        intersection.push_back(*it);
        count.erase(foundIt);
      }
    }

    return intersection;
  }
};

#endif() // MODULE_COMBINATORY_INTERSECTION_HXX
