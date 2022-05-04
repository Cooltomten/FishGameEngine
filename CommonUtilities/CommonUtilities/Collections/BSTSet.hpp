#pragma once

namespace CommonUtilities
{

	template<typename T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();
		//Returnerar true om elementet finns i mängden, och false annars.
		bool HasElement(const T& aValue);
		//Stoppar in elementet i mängden, om det inte redan finns där. Gör
		//ingenting annars.
		void Insert(const T& aValue);
		//Plockar bort elementet ur mängden, om det finns. Gör ingenting annars.
		void Remove(const T& aValue);
	private:
		struct Node
		{
			T Value;
			Node* Left;
			Node* Right;
		};
		void TryInsert(const T& aValue, Node* aNode);
		bool HasElementRecursion(const T& aValue, Node* aNode, int aCount);
		void RemoveRecursion(const T& aValue, Node* aNode);
		void RemoveInternal(Node* aNodeToRemove, Node* aParent, bool aLeftFlag);
		void RemoveAllRecursion(Node* aNode);

		Node* myRootNode;


	};

	template<typename T>
	inline BSTSet<T>::BSTSet()
	{
		myRootNode = nullptr;
	}

	template<typename T>
	inline BSTSet<T>::~BSTSet()
	{

		RemoveAllRecursion(myRootNode);
	}

	template<typename T>
	inline bool BSTSet<T>::HasElement(const T& aValue)
	{
		if (myRootNode == nullptr)
		{
			return false;
		}
		return HasElementRecursion(aValue, myRootNode, 0);
	}

	template<typename T>
	inline void BSTSet<T>::Insert(const T& aValue)
	{
		if (myRootNode == nullptr)
		{
			myRootNode = new Node();
			myRootNode->Value = aValue;
		}
		else
		{
			if (!HasElement(aValue))
			{
				TryInsert(aValue, myRootNode);
			}
		}
	}

	template<typename T>
	inline void BSTSet<T>::Remove(const T& aValue)
	{
		if (myRootNode == nullptr)
		{
			return;
		}
		RemoveRecursion(aValue, myRootNode);
	}

	template<typename T>
	inline void BSTSet<T>::TryInsert(const T& aValue, Node* aNode)
	{
		if (aValue < aNode->Value)
		{
			if (aNode->Left == nullptr)
			{
				aNode->Left = new Node();
				aNode->Left->Value = aValue;
				aNode->Left->Left = nullptr;
				aNode->Left->Right = nullptr;
			}
			else
			{
				TryInsert(aValue, aNode->Left);
			}
		}
		else if (aNode->Value < aValue)
		{
			if (aNode->Right == nullptr)
			{
				aNode->Right = new Node();
				aNode->Right->Value = aValue;
				aNode->Right->Left = nullptr;
				aNode->Right->Right = nullptr;
			}
			else
			{
				TryInsert(aValue, aNode->Right);
			}
		}
	}

	template<typename T>
	inline bool BSTSet<T>::HasElementRecursion(const T& aValue, Node* aNode, int aCount)
	{
		if (aCount == 20)
		{
			int a = 0;
			a;
		}
		if (aValue < aNode->Value)
		{
			if (aNode->Left == nullptr)
			{
				return false;
			}
			else
			{
				return HasElementRecursion(aValue, aNode->Left, ++aCount);
			}
		}
		else if (aNode->Value < aValue)
		{
			if (aNode->Right == nullptr)
			{
				return false;
			}
			else
			{
				return HasElementRecursion(aValue, aNode->Right, ++aCount);
			}
		}
		else
		{
			return true;
		}
	}

	template<typename T>
	inline void BSTSet<T>::RemoveRecursion(const T& aValue, Node* aNode)
	{
		if (aValue < aNode->Value)
		{

			if (!(aValue < aNode->Left->Value) && !(aNode->Left->Value < aValue)) // == operator with <
			{
				RemoveInternal(aNode->Left, aNode, true);
			}
			else
			{
				RemoveRecursion(aValue, aNode->Left);

			}
		}
		else if (aNode->Value < aValue)
		{
			if (!(aValue < aNode->Right->Value) && !(aNode->Right->Value < aValue)) // == operator with <
			{
				RemoveInternal(aNode->Right, aNode, false);
			}
			else
			{
				RemoveRecursion(aValue, aNode->Right);

			}
		}
		else // if root node
		{
			RemoveInternal(aNode, nullptr, false);
		}
	}

	template<typename T>
	inline void BSTSet<T>::RemoveInternal(Node* aNodeToRemove, Node* aParent, bool aLeftFlag)
	{
		if (aNodeToRemove->Left == nullptr && aNodeToRemove->Right == nullptr) // both children are null case
		{
			if (aParent)
			{
				if (aLeftFlag)
				{
					aParent->Left = nullptr;
				}
				else
				{
					aParent->Right = nullptr;
				}
			}
			else
			{
				myRootNode = nullptr;
			}
			delete aNodeToRemove;
		}
		else if ((aNodeToRemove->Left != nullptr && aNodeToRemove->Right == nullptr))
		{

			if (aParent)
			{
				if (aLeftFlag)
				{
					aParent->Left = aNodeToRemove->Left;
				}
				else
				{
					aParent->Right = aNodeToRemove->Left;
				}
			}
			else
			{
				myRootNode = aNodeToRemove->Left;
			}
			delete aNodeToRemove;
		}
		else if ((aNodeToRemove->Left == nullptr && aNodeToRemove->Right != nullptr))
		{


			if (aParent)
			{
				if (aLeftFlag)
				{
					aParent->Left = aNodeToRemove->Right;
				}
				else
				{
					aParent->Right = aNodeToRemove->Right;
				}
			}
			else
			{
				myRootNode = aNodeToRemove->Right;
			}
			delete aNodeToRemove;
		}
		else
		{
			//find rightmost in left tree
			Node* prevNode = aNodeToRemove->Left;
			Node* node = aNodeToRemove->Left;
			while (node->Right != nullptr)
			{
				prevNode = node;
				node = node->Right;
			}
			if (prevNode != node)
			{
				prevNode->Right = node->Left;
				node->Left = aNodeToRemove->Left;
			}
			node->Right = aNodeToRemove->Right;

			if (aParent)
			{
				if (aLeftFlag)
				{
					aParent->Left = node;
				}
				else
				{
					aParent->Right = node;
				}
			}
			else
			{
				myRootNode = node;
			}


			delete aNodeToRemove;


		}
	}

	template<typename T>
	inline void BSTSet<T>::RemoveAllRecursion(Node* aNode)
	{
		if (aNode)
		{
			if (aNode->Left != nullptr)
			{
				RemoveAllRecursion(aNode->Left);
			}
			if (aNode->Right != nullptr)
			{
				RemoveAllRecursion(aNode->Right);
			}
			delete aNode;

		}
	}

}