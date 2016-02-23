#ifndef MODULE_ARRAY_SEARCH_HXX
#define MODULE_ARRAY_SEARCH_HXX

#include "sort.hxx"

// STD includes
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

namespace Collections
{
  /// Binary Search
  /// Iteratively proceed a dichotomic search within a sorted collection on the first occurence
  /// of the key passed as parameter
  ///
  /// @complexity O(log(N))
  ///
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param key the key value to be searched
  ///
  /// @return The vector index of the first found key, -1 otherwise
  template <typename Iterator, typename T>
  int BinarySearch(const Iterator& begin, const Iterator& end, const T& key)
  {
    int index = -1;
    Iterator lowIt = begin;
    Iterator middleIt;
    Iterator highIt = end;

    // While there is still objects between the two iterators and no object has been foud yet
    int middleIdx = std::distance(lowIt, highIt) / 2;
    while(lowIt < highIt && index < 0)
    {
      middleIt = lowIt + middleIdx;

      if (Equal(key, *middleIt))
        // Found object - Set index computed from initial begin iterator
        index = std::distance(begin, middleIt);
      else if (key > *middleIt)
        // Try to find key within upper collection
        lowIt = middleIt + 1;
      else
        // Try to find key within lower collection
        highIt = middleIt;

      middleIdx = std::distance(lowIt, highIt) / 2;
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

    int minValIdx = 0;
    std::pair<int, int> indexes(minValIdx, 1);
    T maxDist = Distance()(*elements.begin(), *(elements.begin() + 1));
    for (std::vector<T>::const_iterator it = elements.begin() + 1; it != elements.end(); ++it)
    {
      const int currentIdx = std::distance(elements.begin(), it);

      // Keeps track of the minimum value index
      if (*it < elements[minValIdx])
        minValIdx = currentIdx;

      // Keeps track of the largest distance and the indexes
      const T distance = Distance()(*it, elements[minValIdx]);
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
  /// - f(a, b) the distance functor used (O(1) for the default std::minus)
  /// - g(a, b) the compare functor used (O(1) for the default std::greater)
  ///
  /// @templateparam Distance functor type computing the distance between two elements
  /// @templateparam Compare functor type
  /// @param elements vector from within the search occurs
  ///
  /// @return indexes of the array with the maximum/minimum sum, <-1,-1> in case of error
  template <typename T, typename Distance /*= std::minus*/, typename Compare /*= std::greater*/>
  std::pair<int, int> MaxSubArray(const std::vector<T>& elements)
  {
    if (elements.size() < 2)
      return std::pair<int, int>(-1, -1);

    int minValIdx = 0;
    std::pair<int, int> indexes(minValIdx, minValIdx);
    T currSum, maxSum;
    currSum = maxSum = *elements.begin();
    T minSum = static_cast<T>(0);
    for (std::vector<int>::const_iterator it = elements.begin() + 1; it != elements.end(); ++it)
    {
      currSum += *it;
      int currentIdx = std::distance(elements.begin(), it);

      // keep track of the minimum sum and its first value index
      if (Compare()(minSum, currSum)) {
        minValIdx = currentIdx;
        minSum = currSum;
      }

      // Keeps track of the maximal sub array and its end value index
      T curMax = Distance()(currSum, minSum);
      if (Compare()(curMax, maxSum)) {
        indexes.first = minValIdx + ((elements[minValIdx] < 0) ? 1 : 0);
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
  /// @param elements a vector from within the search occurs.
  /// @param m the numbers of max elements value to be found.
  ///
  /// @return a vector of sorted in decreasing/increasing order of the m maximum/minimum
  /// elements, an empty array in case of failure.
  template <typename T, typename Compare /*= std::greater_equal*/>
  std::vector<T> MaxMElements(const std::vector<T>& elements, const int m)
  {
    if (m < 1 || m > elements.size())
      return std::vector<T>();

    // Initiale values depends on the comparator functor
    const T limitValue = Compare()(0, std::numeric_limits<T>::lowest()) ?
      std::numeric_limits<T>::lowest() : std::numeric_limits<T>::max();

    std::vector<T> maxMElements(m, limitValue);
    for (std::vector<T>::const_iterator it = elements.begin(); it != elements.end(); ++it)
    {
      // Insert the value at the right place and bubble down replacement value
      T tmpVal = *it;
      for (int i = 0; i < m; ++i)
        if (Compare()(tmpVal, maxMElements[i]))
          std::swap(maxMElements[i], tmpVal);
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
  /// @param begin,end Random-access iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param k the zero-based kth element wanted to be find as.
  ///
  /// @return the kth smallest iterator element of the array, the end iterator in case of failure.
  template <typename Iterator, typename Compare /*= std::less_equal*/>
  Iterator KthMaxElement(Iterator& begin, Iterator& end, size_t k)
  {
    const size_t karraySize = std::distance(begin, end);
    if (k > karraySize || k < 0 || karraySize <= 0)
      return end;

    Iterator pivot = begin + (rand() % (end - begin));            // Take random pivot
    Collections::Partition<Iterator, Compare>(begin, pivot, end); // Partition

    // Get the index of the pivot (i'th smallest/biggest value)
    size_t indexElem = std::distance(begin, pivot);

    // Return if at the kth position
    if (indexElem == k)
      return pivot;

    return (indexElem > k) ? KthMaxElement<Iterator, Compare>(begin, pivot, k)            // Recurse search on left part
                           : KthMaxElement<Iterator, Compare>(pivot, end, k - indexElem); // Recurse search on right part
  }


  /// Intersection - Return Intersection of the two vectors
  /// Retrieve the intersection of two vectors keeping dupplicate keys distinct.
  ///
  /// @complexity O(N)
  ///
  /// @param firstArray the first.
  /// @param secondArray the second.
  ///
  /// @return a vector containing the intersection of both vectors.
  template <typename T>
  std::vector<T> Intersection(const std::vector<T>& firstVector, const std::vector<T>& secondVector)
  {
    // Take the smallest vector for initial count
    const std::vector<T>* smallerVectorPtr = (firstVector.size() >= secondVector.size()) ? &firstVector : &secondVector;
    const std::vector<T>* biggerVectorPtr = (smallerVectorPtr != &firstVector) ? &firstVector : &secondVector;

    std::vector<T> intersection;
    intersection.reserve(smallerVectorPtr->size());

    // Count each element of the smaller array
    std::multiset<T> count;
    for (std::vector<T>::const_iterator it = smallerVectorPtr->begin(); it != smallerVectorPtr->end(); ++it)
        count.insert(*it);

    // Push the element if find into the multiset and delete its instance from the counter
    for (std::vector<T>::const_iterator it = biggerVectorPtr->begin(); it != biggerVectorPtr->end(); ++it)
    {
      std::multiset<T>::iterator itFind = count.find(*it);

      if (itFind != count.end())
      {
        intersection.push_back(*it);
        count.erase(itFind);
      }
    }

    return intersection;
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

  template <>
  bool Equal<>(const char& a, const char& b)
  {
    return a == b;
  }
};

#endif() // MODULE_ARRAY_SEARCH_HXX
