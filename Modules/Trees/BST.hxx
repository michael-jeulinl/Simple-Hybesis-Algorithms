#ifndef MODULE_TREES_BST_HXX
#define MODULE_TREES_BST_HXX

#include <memory>

namespace SHA_Trees
{
  template <class Iterator>
  class BST
  {
    typedef typename Iterator::value_type Value_Type;
    public:
      /// Build - Construct in a naïve way a Binary Search Tree given an unordered sequence of elements.
      ///
      /// @param beginFirst,endFisrt,beginSecond,endSecond - iterators to the initial and final positions of
      /// the sequence used to build the tree. The range used is [first,last), which contains
      /// all the elements between first and last, including the element pointed by first but
      /// not the element pointed by last.
      ///
      /// @complexity O(n * m).
      ///
      /// @return Binary Search Tree pointer to be owned, nullptr if construction failed.
      static std::unique_ptr<BST> Build(const Iterator& begin, const Iterator& end)
      {
        if (begin >= end)
          return  nullptr;

        std::unique_ptr<BST> root = std::unique_ptr<BST>(new BST(*begin));

        // Insert all remaining elements within the tree
        for (Iterator it = begin + 1; it != end; ++it)
          root->Insert(*it);

        return root;
      }

      /// BuildFromSorted - Construct a Balanced Binary Search Tree given an ordered sequence of elements.
      ///
      /// @param beginFirst,endFisrt,beginSecond,endSecond - iterators to the initial and final positions of
      /// the sequence used to build the tree. The range used is [first,last), which contains
      /// all the elements between first and last, including the element pointed by first but
      /// not the element pointed by last.
      ///
      /// @complexity O(n).
      ///
      /// @Warning the algorithm does not check the validity on data order; using this algorithm with
      /// unordored data will most likely result in an invalid BST. (Can be checked using IsValid method).
      ///
      /// @return Binary Search Tree pointer to be owned, nullptr if construction failed.
      static std::unique_ptr<BST> BuildFromSorted(const Iterator& begin, const Iterator& end)
      {
        if (begin >= end)
          return  nullptr;

        const Iterator middle = begin + (std::distance(begin,end) / 2);
        std::unique_ptr<BST> root = std::unique_ptr<BST>(new BST(*middle));

        // Recursively insert both children
        root->SetLeftChild(std::move(BuildFromSorted(begin, middle)));
        root->SetRightChild(std::move(BuildFromSorted(middle + 1, end)));

        return root;
      }

      /// Append Binary Search Tree at the right position.
      ///
      /// @complexity O(n).
      ///
      /// @param data, data value to be added to the current BST.
      void Insert(const Value_Type& data)
      {
        // Key is lower or equal than current root - Insert on the left side
        if (this->data >= data)
        {
          if (this->leftChild == nullptr)
            this->SetLeftChild(std::move(std::unique_ptr<BST>(new BST(data))));
          else
            this->leftChild->Insert(data);
        }
        // Key is greater than current root - Insert on the right side
        else
        {
          if (this->rightChild == nullptr)
            this->SetRightChild(std::move(std::unique_ptr<BST>(new BST(data))));
          else
            this->rightChild->Insert(data);
        }
      }

      /// Check validity of the Binary Search Tree.
      /// Recursively check if subtrees do not violate any of the rules defined by a BST.
      ///
      /// @param bst, the binary search tree to be checked.
      /// Cf. Sutter Guru Of the Week (GOTW) 91 for more information about using raw pointer here.
      ///
      /// @return wheter or not the tree is a valid Binary Search Tree (true) or not (false).
      static bool IsValid(const BST* bst)
      {
        if (!bst)
          return true;

        // Left child exists and has bigger value than its parent - Does not respect BST rules
        if (bst->GetLeftChild() && bst->GetLeftChild()->GetData() > bst->GetData())
          return false;

        // Right child exists and has smaller or equal value of its parent - Does not respect BST rules
        if (bst->GetRightChild() && bst->GetRightChild()->GetData() <= bst->GetData())
          return false;

        // Recursively check subtrees
        return (IsValid(bst->GetLeftChild()) && IsValid(bst->GetRightChild()));
      }

      Value_Type GetData() const { return this->data; }
      const BST* GetLeftChild() const { return this->leftChild.get(); }
      const BST* GetRightChild() const { return this->rightChild.get(); }

    private:
      BST(const Value_Type& data) : data(data) {}
      BST(BST&) {}           // Not Implemented
      BST operator=(BST&) {} // Not Implemented

      void SetLeftChild(std::unique_ptr<BST> bst) { this->leftChild = std::move(bst); }
      void SetRightChild(std::unique_ptr<BST> bst) { this->rightChild = std::move(bst); }

      typename Iterator::value_type data;
      std::unique_ptr<BST> leftChild;
      std::unique_ptr<BST> rightChild;
  };
};

#endif() // MODULE_TREES_BST_HXX
