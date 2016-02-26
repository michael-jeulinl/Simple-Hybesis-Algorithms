#ifndef MODULE_COLLECTIONS_SEARCH_HXX
#define MODULE_COLLECTIONS_SEARCH_HXX

#include "sort.hxx"

// STD includes
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

namespace SHA_Collections
{
  /// Binary Search
  /// Iteratively proceed a dichotomic search within a sorted collection on the first occurence
  /// of the key passed as parameter.
  ///
  /// @complexity O(log(N)).
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param key the key value to be searched.
  ///
  /// @return The vector index of the first found key, -1 otherwise.
  template <typename Iterator, typename T, typename IsEqual>
  int BinarySearch(const Iterator& begin, const Iterator& end, const T& key)
  {
    int index = -1;
    Iterator lowIt = begin;
    Iterator highIt = end;
    Iterator middleIt = lowIt + std::distance(lowIt, highIt) / 2;

    // While there is still objects between the two iterators and no object has been foud yet
    while(lowIt < highIt && index < 0)
    {
      if (IsEqual()(key, *middleIt))
        // Found object - Set index computed from initial begin iterator
        index = static_cast<int>(std::distance(begin, middleIt));
      else if (key > *middleIt)
        // Try to find key within upper collection
        lowIt = middleIt + 1;
      else
        // Try to find key within lower collection
        highIt = middleIt;

      middleIt = lowIt + std::distance(lowIt, highIt) / 2;
    }

    return index;
  }


  /// MaxDistance
  /// Identifies the two indexes of the array with the maximal distance.
  ///
  /// @detail Known as the simple stock market problem with the default functor (std::minus):
  /// It finds i and j that maximizes Aj – Ai, where i < j.
  /// In other words, maximizes the benefice of a resell given an array of prices varying over time.
  ///
  /// @complexity O(N * O(f(a, b))), with f(a,b) the functor used; is O(1) for the default std::minus.
  ///
  /// @templateparam Distance functor type computing the distance between two elements
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return indexes of the array with the maximal distance, <-1,-1> in case of error.
  template <typename Iterator, typename Distance /*= std::minus*/>
  std::pair<int, int> MaxDistance(const Iterator& begin, const Iterator& end)
  {
    if (std::distance(begin, end) < 2)
      return std::pair<int, int>(-1, -1);

    int minValIdx = 0;
    std::pair<int, int> indexes(minValIdx, 1);
    Iterator::value_type maxDist = Distance()(*begin, *(begin + 1));
    for (Iterator it = begin + 1; it != end; ++it)
    {
      const int currentIdx = static_cast<const int>(std::distance(begin, it));

      // Keeps track of the minimum value index
      if (*it < *(begin + minValIdx))
        minValIdx = currentIdx;

      // Keeps track of the largest distance and the indexes
      const Iterator::value_type distance = Distance()(*it, *(begin + minValIdx));
      if (distance > maxDist) {
        maxDist = distance;
        indexes.first = minValIdx;
        indexes.second = currentIdx;
      }
    }

    return indexes;
  }


  /// Max Subarray
  /// Identify the subarray with the maximum/minimum sum.
  ///
  /// @detail can be seen as an elicitation of the @MaxDistance one.
  /// One of the problem resolved by this algorithm is:
  /// "Given an array of gains/losses over time, find the period that represents the best/worst
  /// cummulative gain."
  /// The algorithm uses the fact that the sum operator is a transitive function.
  ///
  /// @complexity O(N * (O(f(a, b)) + O(g(a, b))) with:
  /// - f(a, b) the distance functor used; is O(1) for the default std::minus.
  /// - g(a, b) the compare functor used; is O(1) for the default std::greater.
  ///
  /// @templateparam Distance functor type computing the distance between two elements
  /// @templateparam Compare functor type
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return indexes of the array with the maximum/minimum sum, <-1,-1> in case of error.
  template <typename Iterator, typename Distance /*= std::minus*/, typename Compare /*= std::greater*/>
  std::pair<int, int> MaxSubArray(const Iterator& begin, const Iterator& end)
  {
    if (std::distance(begin, end) < 2)
      return std::pair<int, int>(-1, -1);

    int minValIdx = 0;
    std::pair<int, int> indexes(minValIdx, minValIdx);
    Iterator::value_type currSum, maxSum;
    currSum = maxSum = *begin;
    Iterator::value_type minSum = static_cast<Iterator::value_type>(0);
    for (Iterator it = begin + 1; it != end; ++it)
    {
      currSum += *it;
      int currentIdx = static_cast<int>(std::distance(begin, it));

      // keep track of the minimum sum and its first value index
      if (Compare()(minSum, currSum)) {
        minValIdx = currentIdx;
        minSum = currSum;
      }

      // Keeps track of the maximal sub array and its end value index
      Iterator::value_type curMax = Distance()(currSum, minSum);
      if (Compare()(curMax, maxSum)) {
        indexes.first = minValIdx + ((*(begin + minValIdx) < 0) ? 1 : 0);
        indexes.second = currentIdx;
        maxSum = Distance()(currSum, minSum);
      }
    }

    return indexes;
  }


  /// Max M Elements
  /// Identify the m maximal/minimal values sorted in decreasing/increasing order.
  ///
  /// @detail using this algorithm with the size of the vector as the number
  /// of elements to be found will give you a bubble sort algorithm in O(N²).
  ///
  /// @complexity O(N * m * O(f(a, b))) with:
  /// - m number of elements to be looking for
  /// - f(a, b) the compare functor used (O(1) for the default std::greater_equal)
  ///
  /// @templateparam Compare functor type
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param m the numbers of max elements value to be found.
  ///
  /// @return a vector of sorted in decreasing/increasing order of the m maximum/minimum
  /// elements, an empty array in case of failure.
  template <typename Container, typename Iterator, typename Compare /*= std::greater_equal*/>
  Container MaxMElements(const Iterator& begin, const Iterator& end, const int m)
  {
    if (m < 1 || m > std::distance(begin, end))
      return Container();

    // Initiale values depends on the comparator functor
    const Iterator::value_type limitValue =
      Compare()(0, std::numeric_limits<Iterator::value_type>::lowest()) ?
        std::numeric_limits<Iterator::value_type>::lowest() :
        std::numeric_limits<Iterator::value_type>::max();

    // Allocate the container final size
    Container maxMElements;
    maxMElements.resize(m, limitValue);
    for (Iterator it = begin; it != end; ++it)
    {
      // Insert the value at the right place and bubble down replacement value
      int index = 0;
      Iterator::value_type tmpVal = *it;
      for (Container::iterator subIt = maxMElements.begin(); index < m; ++subIt, ++index)
        if (Compare()(tmpVal, *subIt))
          std::swap(*subIt, tmpVal);
    }

    return maxMElements;
  }


  /// Kth Smallest / Biggest element - Order Statitstics
  /// Find the kth smallest/biggest element contained within [begin, end[.
  ///
  /// @warning this method is not stable (does not keep order with element of the same value).
  /// @warning this method changes the elements order between your iterators.
  ///
  /// @complexity recursively look on the choosen partition: N + N/2 + N/4 + N/8 + ... = O(N)
  ///
  /// @templateparam Random-access iterator type
  /// @templateparam Compare functor type (std::less_equal to find kth smallest element,
  /// std::greater_equal to find the kth biggest one)
  /// @param begin,end - iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param k the zero-based kth element - 0 for the biggest/smallest.
  ///
  /// @return the kth smallest iterator element of the array, the end iterator in case of failure.
  template <typename Iterator, typename Compare /*= std::less_equal*/>
  Iterator KthMaxElement(Iterator& begin, Iterator& end, size_t k)
  {
    const int kSize = static_cast<const int>(std::distance(begin, end));
    if (k > kSize || k < 0 || kSize <= 0)
      return end;

    Iterator pivot = begin + (rand() % (end - begin));                // Take random pivot
    SHA_Collections::Partition<Iterator, Compare>(begin, pivot, end); // Partition

    // Get the index of the pivot (i'th smallest/biggest value)
    const size_t kPivotIndex = std::distance(begin, pivot);

    // Return if at the kth position
    if (kPivotIndex == k)
      return pivot;

    // Recurse search on left part if there is more than k elements within the left sequence
    // Recurse search on right otherwise
    return (kPivotIndex > k) ? KthMaxElement<Iterator, Compare>(begin, pivot, k)
                             : KthMaxElement<Iterator, Compare>(pivot, end, k - kPivotIndex);
  }


  /// Intersection - Return Intersection of the two vectors
  /// Retrieve the intersection of two vectors keeping dupplicate keys distinct.
  ///
  /// @complexity O(N + M)
  ///
  /// @param beginFirst,endFisrt,beginSecond,endSecond - iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last..
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
    for (Container::const_iterator
      it = (kIsFirstSmaller) ? beginFirst : beginSecond;
      it != kCountEndIt; ++it)
        count.insert(*it);

    // Push the element if find into the multiset and delete its instance from the counter
    const Iterator kIntersectionEndIt = (kIsFirstSmaller) ? endSecond : endFirst;
    for (Container::const_iterator
      it = (kIsFirstSmaller) ? beginSecond : beginFirst;
      it != kIntersectionEndIt; ++it)
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

#endif() // MODULE_COLLECTIONS_SEARCH_HXX
