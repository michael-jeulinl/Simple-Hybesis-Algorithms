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
  /// @param sortedArray a sorted vector from within the search occurs
  /// @param key the key value to be searched
  ///
  /// @complexity O(log(N))
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
  /// @param elements vector from within the search occurs
  /// @param template functor type computing the distance between two elements
  ///
  /// @complexity O(N)
  /// @return indexes of the array with the maximal distance, <-1,-1> in case of error
  template <typename T, typename Distance /*= std::minus*/>
  std::pair<int, int> MaxDistance(const std::vector<T>& elements)
  {
    if (elements.size() < 2)
      return std::pair<int, int>(-1, -1);

    const Distance distanceFunc = Distance();
    std::pair<int, int> indexes(0, 0);
    T maxDist = std::numeric_limits<T>::lowest();
    for (std::vector<T>::const_iterator it = elements.begin() + 1; it != elements.end(); ++it)
    {
      const int currentIdx = std::distance(elements.begin(), it);

      // Keeps track index of the minimum value
      if (*it < elements[indexes.first])
        indexes.first = currentIdx;

      // Keeps track of the largest distance and its max value index
      const T distance = distanceFunc(*it, elements[indexes.first]);
      if (distance > maxDist) {
        maxDist = distance;
        indexes.second = currentIdx;
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