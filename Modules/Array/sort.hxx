#ifndef MODULE_ARRAY_SORT_HXX
#define MODULE_ARRAY_SORT_HXX

// STD includes
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

namespace ArrayAlgorithms
{
  /// Partition-Exchange
  /// Proceed an in-place patitionning on the elements 
  ///
  /// @complexity O(N)
  ///
  /// @templateparam Random-access iterator type
  /// @templateparam Compare functor type (std::less_equal for smaller elements in left partition,
  /// std::greater_equal for greater elements in left partition)
  /// @param begin,end const Random-access iterators to the initial and final positions of 
  /// the sequence to be pivoted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param pivot Random-access iterators to position between begin and end.
  ///
  /// @return void.
  template <typename Iterator, typename Compare /*= std::less_equal*/>
  void Partition(const Iterator& begin, Iterator& pivot, const Iterator& end)
  {
    if (std::distance(begin, end) < 2 || pivot == end)
      return;

    Iterator::value_type pivotValue = *pivot;  // Keep the pivot value;
    std::swap(*pivot, *(end - 1));             // Put the pivot at the end for convenience
    Iterator store = begin;                    // Put the store pointer at the beginning

    // Swap each smaller before the pivot item
    for (Iterator it = begin; it != end - 1; ++it)
    {
      if (Compare()(*it, pivotValue))
      {
        std::swap(*store, *it);
        ++store;
      }
    }

    pivot = store;                 // Return the pointer on the pivot
    std::swap(*(end - 1), *pivot); // Replace the pivot at its good position
  }


  /// Quick Sort - Partition-Exchange Sort
  /// Proceed an in-place quick-sort on the elements
  ///
  /// @complexity O(N * log(N)) in average case and O(N²) on worst case.
  /// Note: this algorithm performs in general 2 to 3 time faster than a classic merge sort.
  /// Note: this algorithm is easily parallelizable.
  ///
  /// @templateparam Random-access iterator type
  /// @templateparam Compare functor type (std::less_equal for in order, std::greater_equal for inverse order)
  /// @param begin,end Random-access iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Iterator, typename Compare /*= std::less_equal*/>
  void QuickSort(Iterator& begin, Iterator& end)
  {
    int distance = std::distance(begin, end);
    if (distance < 2)
      return;

    Iterator pivot = begin + (rand() % (end - begin));  // Pick Random Pivot € [begin, end]
    Partition<Iterator, Compare>(begin, pivot, end);    // Proceed partition

    QuickSort<Iterator, Compare>(begin, pivot);   // Recurse on first partition
    QuickSort<Iterator, Compare>(pivot + 1, end); // Recurse on second partition
  }
};

#endif() // MODULE_ARRAY_SORT_HXX
