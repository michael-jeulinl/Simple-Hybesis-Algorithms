#ifndef MODULE_ARRAY_SEARCH_HXX
#define MODULE_ARRAY_SEARCH_HXX

// STD includes
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

namespace ArrayAlgorithms
{
	/// Binary Search - Iterative
  /// Search within a sorted vector the occurence of the key passed as parameter
  ///
  /// @param sortedArray a sorted vector from within the search occurs
  /// @param key the key value to be searched
  ///
  /// @complexity O(log(N))
  /// @return The vector index of the found key, -1 otherwise
  template <class T>
	int BinarySearchIterativeT(const std::vector<T>& sortedArray, const T& key)
  {
    int index = -1;
	  int mid; 
	  int lowInd = 0; 
	  int highInd = sortedArray.size() - 1; 

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