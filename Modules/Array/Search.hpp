#ifndef MODULE_ARRAY_SEARCH_HXX
#define MODULE_ARRAY_SEARCH_HXX

// STD includes
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

namespace ArrayAlgorithms
{
  /// Binary Search - Iterative
  /// Proceed a dichotomic search within a sorted vector the first occurence
  /// of the key passed as parameter
  ///
  /// @complexity O(log(N))
  ///
  /// @param sortedArray a sorted vector from within the search occurs
  /// @param key the key value to be searched
  ///
  /// @return The vector index of the first found key, -1 otherwise
  template <typename T>
  int BinarySearchIterativeT(const std::vector<T>& sortedArray, const T& key)
  {
    int index = -1;
    int mid;
    int lowInd = 0;
    int highInd = static_cast<int>(sortedArray.size() - 1);

    while(lowInd <= highInd && index < 0)
    {
      mid = (lowInd + highInd) / 2;

      if (Equal(key, sortedArray[mid]))
        index = mid;
      else if (key > sortedArray[mid])
        lowInd = mid + 1;
      else
        highInd = mid - 1;
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
  /// @complexity O(N * O(f(a, b))), with f(a,b) the functor used (O(1) for the default std::minus)
  ///
  /// @templateparam Distance functor type computing the distance between two elements
  /// @param elements vector from within the search occurs
  ///
  /// @return indexes of the array with the maximal distance, <-1,-1> in case of error
  template <typename T, typename Distance /*= std::minus*/>
  std::pair<int, int> MaxDistance(const std::vector<T>& elements)
  {
    if (elements.size() < 2)
      return std::pair<int, int>(-1, -1);

    const Distance distanceFunc = Distance();

    int minValIdx = 0;
    std::pair<int, int> indexes(minValIdx, 1);
    T maxDist = distanceFunc(*elements.begin(), *(elements.begin() + 1));
    for (std::vector<T>::const_iterator it = elements.begin() + 1; it != elements.end(); ++it)
    {
      const int currentIdx = std::distance(elements.begin(), it);

      // Keeps track of the minimum value index
      if (*it < elements[minValIdx])
        minValIdx = currentIdx;

      // Keeps track of the largest distance and the indexes
      const T distance = distanceFunc(*it, elements[minValIdx]);
      if (distance > maxDist) {
        maxDist = distance;
        indexes.first = minValIdx;
        indexes.second = currentIdx;
      }
    }

    return indexes;
  }

  /// Maximal Subarray
  /// Identify the subarray with the maximum/minimum sum.
  ///
  /// @detail can be seen as an elicitation of the @MaxDistance one.
  /// One of the problem resolved by this algorithm is:
  /// "Given an array of gains/losses over time, find the period that represents the best/worst
  /// cummulative gain."
  /// The algorithm uses the fact that the sum operator is a transitive function.
  ///
  /// @complexity O(N * (O(f(a, b)) + O(g(a, b))):
  /// - With f(a, b) the distance functor used (O(1) for the default std::minus)
  /// - With g(a, b) the compare functor used (O(1) for the default std::greater)
  ///
  /// @templateparam Distance functor type computing the distance between two elements
  /// @templateparam Compare functor type
  /// @param elements vector from within the search occurs
  ///
  /// @return indexes of the array with the maximum/minimum sum, <-1,-1> in case of error
  template <typename T, typename Distance /*= std::minus*/, typename Compare /*= std::greater*/>
  std::pair<int, int> MaximalSubArray(const std::vector<T>& elements)
  {
    if (elements.size() < 2)
      return std::pair<int, int>(-1, -1);

    const Distance distanceFunc = Distance();
    const Compare compareFunc = Compare();

    int minValIdx = 0;
    std::pair<int, int> indexes(minValIdx, minValIdx);
    T currSum, maxSum;
    currSum = maxSum = *elements.begin();
    T minSum = static_cast<T>(0);
    for (std::vector<int>::const_iterator it = elements.begin() + 1; it != elements.end(); ++it)
    {
      currSum += *it;
      const int currentIdx = std::distance(elements.begin(), it);

      // keep track of the minimum sum and its first value index
      if (compareFunc(minSum, currSum)) {
        minValIdx = currentIdx;
        minSum = currSum;
      }

      // Keeps track of the maximal sub array and its end value index
      T curMax = distanceFunc(currSum, minSum);
      if (compareFunc(curMax, maxSum)) {
        indexes.first = minValIdx + ((elements[minValIdx] < 0) ? 1 : 0);
        indexes.second = currentIdx;
        maxSum = distanceFunc(currSum, minSum);
      }
    }

    return indexes;
  }

  template <class T>
  bool Equal(const T& a, const T& b) 
  {
    return std::abs(a - b) < std::numeric_limits<T>::epsilon();
  }

  template <>
  bool Equal<>(const int& a, const int& b) 
  {
    return a == b;
  }
};

#endif() // MODULE_ARRAY_SEARCH_HXX