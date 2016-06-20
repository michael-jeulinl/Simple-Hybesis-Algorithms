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
      /// Build - Construct in a naïve way a Binary Search Tree an unordered sequence of elements.
      ///
      /// @param beginFirst,endFisrt,beginSecond,endSecond - iterators to the initial and final positions of
      /// the sequence used to build the tree. The range used is [first,last), which contains
      /// all the elements between first and last, including the element pointed by first but
      /// not the element pointed by last.
      ///
      /// @return Binary Search Tree pointer to be owned, nullptr if construction failed.
      static std::unique_ptr<BST> Build(const Iterator& begin, const Iterator& end)
      {
        if (begin >= end)
          return  nullptr;

        std::unique_ptr<BST> root = std::unique_ptr<BST>(new BST(*begin));

        // Insert all remaining elements within the tree
        for (Iterator it = begin + 1; it != end; ++it)
          root->Append(std::move(std::unique_ptr<BST>(new BST(*it))));

        return root;
      }

      /// Append Binary Search Tree at the right position.
      ///
      /// @param bst, the binary search tree unique_ptr to be owned and appened as a subtree.
      ///
      /// @Note Takes the ownership of the unique_ptr<BST> passed as argument.
      void Append(std::unique_ptr<BST> bst)
      {
        if (!bst)
          return;

        // Key is lower or equal than current root - Insert on the left side
        if (this->data >= bst->GetData())
        {
          if (this->leftChild == nullptr)
            this->SetLeftChild(std::move(bst));
          else
            this->leftChild->Append(std::move(bst));
        }
        // Key is greater than current root - Insert on the right side
        else
        {
          if (this->rightChild == nullptr)
            this->SetRightChild(std::move(bst));
          else
            this->rightChild->Append(std::move(bst));
        }
      }

      Value_Type GetData() const { return this->data; }
      const BST* GetLeftChild() const { return this->leftChild.get(); }
      const BST* GetRightChild() const { return this->rightChild.get(); }

    private:
      BST(Value_Type data) : data(data) {}
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
