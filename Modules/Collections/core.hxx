#ifndef MODULE_COLLECTIONS_CORE_HXX
#define MODULE_COLLECTIONS_CORE_HXX

#include <list>
#include <map>
#include <set>

namespace SHA_Collections
{
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


/// Permutations - Return all possible permutations of the sequence elements.
///
/// @complexity O(N!)
/// @Note a vector is not recommended as type for the Output_Container to avoid stack overflow as well
/// as extra complexity due to frequent resizing (use instead structure such as list or a another with
/// your own allocator).
///
/// @param beginFirst,endFisrt - iterators to the initial and final positions of
/// the sequence to be sorted. The range used is [first,last), which contains all the elements between
/// first and last, including the element pointed by first but not the element pointed by last..
///
/// @return a collection containing all possible permuted sequences.
template <typename Container, typename Iterator>
std::list<Container> Permutations(const Iterator& begin, const Iterator& end)
{
  std::list<Container> permutations; // Contains the output permutations

  // Recusion termination
  const int kSeqSize = static_cast<const int>(std::distance(begin, end));
  if (kSeqSize <= 0)
  {
    return permutations;
  }
  else if (kSeqSize == 1)
  {
    Container elementContainer;
    elementContainer.push_back(*begin);
    permutations.push_back(elementContainer);
    return permutations;
  }

  // Build all permutations of the suffix - Recursion
  std::list<Container> subPermutations = Permutations<Container, Iterator>(begin + 1, end);

  // Put the letter into every possible position of the existing permutations.
  Container currentPermutation;
  for (std::list<Container>::iterator it = subPermutations.begin(); it != subPermutations.end(); ++it)
    for (int i = 0; i < kSeqSize; ++i)
    {
      currentPermutation = *it;
      currentPermutation.insert(currentPermutation.begin() + i, *begin);
      permutations.push_back(currentPermutation);
    }

  return permutations;
}

/// Combinations - Return all possible combinations of the sequence.
///
/// @complexity O((n,n-k) k=1;n)
/// @Note a vector is not recommended as type for the Output_Container to avoid stack overflow as well
/// as extra complexity due to frequent resizing (use instead structure such as list or a another with
/// your own allocator).
///
/// @param beginFirst,endFisrt - iterators to the initial and final positions of
/// the sequence to be sorted. The range used is [first,last), which contains all the elements between
/// first and last, including the element pointed by first but not the element pointed by last..
///
/// @return a collection containing all possible combined sequences.
template <typename Container, typename Iterator>
std::list<Container> Combinations(const Iterator& begin, const Iterator& end)
{
	std::list<Container> combinations;

	// Recusion termination
	const int kSeqSize = static_cast<const int>(std::distance(begin, end));
	if (kSeqSize <= 0)
		return combinations;

  // Keep first element
  Container elementContainer;
  elementContainer.push_back(*begin);
  combinations.push_back(elementContainer);

  // Recursion termination
	if (kSeqSize == 1)
		return combinations;

	// Build all permutations of the suffix - Recursion
	std::list<Container> subCombinations = Combinations<Container, Iterator>(begin + 1, end);

	// Put the letter into every possible position of the existing permutations.
	Container currentPermutation;
	for (std::list<Container>::iterator it = subCombinations.begin(); it != subCombinations.end(); ++it)
	{
    combinations.push_back(*it);
    it->push_back(*begin);
    combinations.push_back(*it);
	}

	return combinations;
}

};

#endif() // MODULE_COLLECTIONS_CORE_HXX
