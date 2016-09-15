#ifndef MODULE_COLLECTIONS_SORT_HXX
#define MODULE_COLLECTIONS_SORT_HXX

// STD includes
#include <functional>
#include <limits>
#include <queue>
#include <vector>

namespace SHA_Collections
{
  /// Partition-Exchange
  /// Proceed an in-place patitionning on the elements 
  ///
  /// @complexity O(N)
  ///
  /// @tparam iterator type
  /// @tparam Compare functor type (std::less_equal for smaller elements in left partition,
  /// std::greater_equal for greater elements in left partition)
  /// @param begin,end const iterators to the initial and final positions of
  /// the sequence to be pivoted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param pivot iterators to position between begin and end.
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
  /// @tparam iterator type
  /// @tparam Compare functor type (std::less_equal in order, std::greater_equal for inverse order)
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Iterator, typename Compare  /*= std::less_equal*/>
  void QuickSort(Iterator& begin, Iterator& end)
  {
    const int distance = static_cast<const int>(std::distance(begin, end));
    if (distance < 2)
      return;

    Iterator pivot = begin + (rand() % (end - begin));  // Pick Random Pivot € [begin, end]
    Partition<Iterator, Compare>(begin, pivot, end);    // Proceed partition

    QuickSort<Iterator, Compare>(begin, pivot);   // Recurse on first partition
    QuickSort<Iterator, Compare>(pivot + 1, end); // Recurse on second partition
  }


  /// MergeInplace - Inplace merging of two ordered sequences of a collection
  /// Proceed a in place merge of two sequences of elements contained in [begin, middle[ and [middle, end[.
  ///
  /// @warning Both sequence [bengin, middle[ and [middle, end[ need to be ordered.
  ///
  /// @remark use MergeWithBuffer to proceed the merge using a buffer:
  /// Takes higher memory consumption and lower computation consumption.
  ///
  /// @complexity O(N * M)
  ///
  /// @tparam iterator type
  /// @param begin,middle,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Iterator>
  class MergeInPlace
  {
  public:
    void operator()(Iterator begin, Iterator middle, Iterator end)
    {
      if (std::distance(begin, middle) < 1 || std::distance(middle, end) < 1)
        return;

      // Use first half as receiver
      for(; begin < middle; ++begin)
      {
        if (*middle >= *begin)
          continue;

        Iterator::value_type value;
        std::swap(value, *begin);    // keep the higher value
        std::swap(*begin, *middle);  // Place it at the beginning of the second list

        // Displace the higher value in the right place of the second list by swapping
        Iterator it = middle;
        for (; it != end - 1; ++it)
        {
          if (*(it + 1) >= value)
            break;

          std::swap(*it, *(it + 1));
        }

        // Restore the value at his right place
        std::swap(*it, value);
      }
    }
  };


  /// MergeWithBuffer - Merging of two ordered sequences of a collection using intermediate buffer
  /// Proceed a merge of two sequences of elements contained in [begin, middle[ and [middle, end[.
  ///
  /// @warning Both sequence [bengin, middle[ and [middle, end[ need to be ordered.
  ///
  /// @remark use MergeInPlace to proceed the merge in place:
  /// Takes lower memory consumption and higher computation consumption.
  ///
  /// @complexity O(N)
  ///
  /// @tparam iterator type
  /// @param begin,middle,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Container, typename Iterator>
  class MergeWithBuffer
  {
  public:
    void operator()(Iterator begin, Iterator middle, Iterator end)
    {
      if (std::distance(begin, middle) < 1 || std::distance(middle, end) < 1)
        return;

      Container buffer;
      buffer.resize(std::distance(begin, end)); // Reserve right space (method working with string container)
      Iterator buffIt = buffer.begin();
      Iterator tmpBegin = begin;

      // Merge into the buffer array taking one by one the lowest sequence element
      const Iterator curMiddle(middle);
      while (begin != curMiddle && middle != end)
      {
        if (*begin <= *middle)
          *buffIt++ = *begin++;
        else
          *buffIt++ = *middle++;
      }

      // Finish both list into the buffer
      for (; begin != curMiddle; ++buffIt, ++begin)
        *buffIt = *begin;
      for (; middle != end; ++buffIt, ++middle)
        *buffIt = *middle;

      // Refill array given the right position
      for (buffIt = buffer.begin(); buffIt != buffer.end(); ++buffIt, ++tmpBegin)
        *tmpBegin = *buffIt;
    }
  };


  /// MergeSort - John von Neumann in 1945
  /// Proceed merge-sort on the elements whether using an in-place strategy or using a buffer.
  ///
  /// @complexity O(N * log(N))
  ///
  /// @tparam iterator type
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  ///
  /// @return void.
  template <typename Container, typename Iterator, typename Aggregator>
  void MergeSort(Iterator& begin, Iterator& end)
  {
    const int ksize = static_cast<const int>(std::distance(begin, end));
    if (ksize < 2)
      return;

    Iterator middle = begin + ksize / 2;

    // Recursively break the vector into two pieces
    SHA_Collections::MergeSort<Container, Iterator, Aggregator>(begin, middle);
    SHA_Collections::MergeSort<Container, Iterator, Aggregator>(middle, end);

    // Merge the two pieces
    Aggregator()(begin, middle, end);
  }


  /// LSD Raddix Sort - Non-comparative integer sorting algorithm
  /// Proceed a raddix-sort on the elements contained in [begin, end[
  ///
  /// @warning Works properly only with integral type of non-negative values
  ///
  /// @complexity O(d * N) with d max number of digits.
  ///
  /// @tparam iterator type
  /// @param begin,end iterators to the initial and final positions of
  /// the sequence to be sorted. The range used is [first,last), which contains all the elements between
  /// first and last, including the element pointed by first but not the element pointed by last.
  /// @param base base in which the numbers are represented
  ///
  /// @return void.
  template <typename Iterator>
  void RaddixSort(Iterator& begin, Iterator& end, unsigned int base = 10)
  {
    if (std::distance(begin, end) < 2)
      return;

    // Create a bucket for each possible value of a digit
    std::vector<std::queue<Iterator::value_type> > buckets(base);

    // For all possible digit
    for (size_t powBase = 1; powBase < std::numeric_limits<Iterator::value_type>::max(); powBase *= base)
    {
      // Push each number into the bucket of its digit value
      for (Iterator it = begin; it != end; ++it)
        buckets[static_cast<int>(*it / powBase) % base].push(*it);

      // Dequeu back all the values
      Iterator itSrc = begin;
      for (std::vector<std::queue<int> >::iterator it = buckets.begin(); it != buckets.end(); ++it)
        while (!it->empty())
        {
          *(itSrc++) = it->front();
          it->pop();
        }
    }
}
};

#endif() // MODULE_COLLECTIONS_SORT_HXX
