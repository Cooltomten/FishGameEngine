#pragma once

namespace CommonUtilities
{
	template<class T>
	class DoublyLinkedList;

	template <class T>
	class DoublyLinkedListNode
	{
	public:
		// Copy-konstruktorn och assignment-operatorn är borttagna, så att det enda
		// sättet att skapa en nod är genom att stoppa in ett värde i en lista.
		DoublyLinkedListNode<T>(const DoublyLinkedListNode<T>&) = delete;
		DoublyLinkedListNode<T>& operator=(const DoublyLinkedListNode<T>&) = delete;
		// Returnerar nodens värde
		const T& GetValue() const;
		T& GetValue();
		// Returnerar nästa nod i listan, eller nullptr om noden är sist i listan
		DoublyLinkedListNode<T>* GetNext() const;
		// Returnerar föregående nod i listan, eller nullptr om noden är först i
		// listan
		DoublyLinkedListNode<T>* GetPrevious() const;
	private:
		// Konstruktorn och destruktorn är privat, så att man inte kan skapa eller
		// ta bort noder utifrån. List-klassen är friend, så att den kan skapa
		// eller ta bort noder.
		friend class DoublyLinkedList<T>;
		DoublyLinkedListNode(const T& aValue);
		~DoublyLinkedListNode() {}

		T myValue;
		DoublyLinkedListNode<T>* myNext;
		DoublyLinkedListNode<T>* myPrevious;

	};

	template <class T>
	class DoublyLinkedList
	{
	public:
		// Skapar en tom lista
		DoublyLinkedList();
		// Frigör allt minne som listan allokerat
		~DoublyLinkedList();
		// Returnerar antalet element i listan
		int GetSize() const;
		// Returnerar första noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetFirst();
		// Returnerar sista noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetLast();
		// Skjuter in ett nytt element först i listan
		void InsertFirst(const T& aValue);
		// Skjuter in ett nytt element sist i listan
		void InsertLast(const T& aValue);
		// Skjuter in ett nytt element innan aNode
		void InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue);
		// Skjuter in ett nytt element efter aNode
		void InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue);
		// Plockar bort noden ur listan och frigör minne. (Det är ok att anta att
		// aNode är en nod i listan, och inte från en annan lista)
		void Remove(DoublyLinkedListNode<T>* aNode);
		// Hittar första elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindFirst(const T& aValue);
		// Hittar sista elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindLast(const T& aValue);
		// Plockar bort första elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveFirst(const T& aValue);
		// Plockar bort sista elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveLast(const T& aValue);

	private:
		DoublyLinkedListNode<T>* myFirst;
		DoublyLinkedListNode<T>* myLast;
		int mySize;
	};

	template<class T>
	const T& DoublyLinkedListNode<T>::GetValue() const
	{
		return myValue;
	}

	template<class T>
	T& DoublyLinkedListNode<T>::GetValue()
	{
		return myValue;
	}

	template<class T>
	DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetNext() const
	{
		return myNext;
	}

	template<class T>
	DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetPrevious() const
	{
		return myPrevious;
	}

	template<class T>
	DoublyLinkedListNode<T>::DoublyLinkedListNode(const T& aValue)
	{
		myValue = aValue;
		myNext = nullptr;
		myPrevious = nullptr;
	}

	template<class T>
	DoublyLinkedList<T>::DoublyLinkedList()
	{
		mySize = 0;
		myFirst = nullptr;
		myLast = nullptr;
	}

	template<class T>
	DoublyLinkedList<T>::~DoublyLinkedList()
	{
		if (myFirst != nullptr)
		{
			DoublyLinkedListNode<T>* node = myFirst;
			while (node->myNext != nullptr) // move to next then delete previous
			{
				node = node->myNext;
				delete node->myPrevious;
			}
			delete node; // delete the last
		}
	}

	template<class T>
	int DoublyLinkedList<T>::GetSize() const
	{
		return mySize;
	}

	template<class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetFirst()
	{
		return myFirst;
	}

	template<class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetLast()
	{
		return myLast;
	}

	template<class T>
	void DoublyLinkedList<T>::InsertFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* node = new DoublyLinkedListNode<T>(aValue);
		node->myNext = myFirst;

		if (myFirst != nullptr)
		{
			myFirst->myPrevious = node;
		}

		myFirst = node;

		if (myLast == nullptr)
		{
			myLast = node;
		}

		mySize++;
	}

	template<class T>
	void DoublyLinkedList<T>::InsertLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* node = new DoublyLinkedListNode<T>(aValue);
		node->myPrevious = myLast;
		if (myLast != nullptr)
		{
			myLast->myNext = node;
		}
		myLast = node;

		if (myFirst == nullptr)
		{
			myFirst = node;
		}
		mySize++;
	}

	template<class T>
	void DoublyLinkedList<T>::InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		DoublyLinkedListNode<T>* node = new DoublyLinkedListNode<T>(aValue);
		node->myNext = aNode;
		node->myPrevious = aNode->myPrevious;
		if (node->myPrevious != nullptr)
		{
			node->myPrevious->myNext = node;
		}
		aNode->myPrevious = node;

		if (node->myPrevious == nullptr)
		{
			myFirst = node;
		}
		mySize++;
	}

	template<class T>
	void DoublyLinkedList<T>::InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		DoublyLinkedListNode<T>* node = new DoublyLinkedListNode<T>(aValue);
		node->myNext = aNode->myNext;
		node->myPrevious = aNode;
		if (node->myNext != nullptr)
		{
			node->myNext->myPrevious = node;
		}
		aNode->myNext = node;

		if (node->myNext == nullptr)
		{
			myLast = node;
		}

		mySize++;
	}

	template<class T>
	void DoublyLinkedList<T>::Remove(DoublyLinkedListNode<T>* aNode)
	{
		if (mySize == 1)
		{
			myFirst = nullptr;
			myLast = nullptr;
		}
		else
		{
			if (aNode != myFirst)
			{
				aNode->myPrevious->myNext = aNode->myNext;
			}
			if (aNode != myLast)
			{
				aNode->myNext->myPrevious = aNode->myPrevious;
			}

			if (aNode->myPrevious == nullptr)
			{
				myFirst = aNode->myNext;
			}
			if (aNode->myNext == nullptr)
			{
				myLast = aNode->myPrevious;
			}
		}
		delete aNode;
		mySize--;
	}

	template<class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* node = myFirst;
		while (node != nullptr)
		{
			if (node->myValue == aValue)
			{
				break;
			}
			node = node->myNext;
		}
		return node;
	}

	template<class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* node = myLast;
		while (node != nullptr)
		{
			if (node->myValue == aValue)
			{
				break;
			}
			node = node->myPrevious;
		}
		return node;
	}

	template<class T>
	bool DoublyLinkedList<T>::RemoveFirst(const T& aValue)
	{
		auto first = FindFirst(aValue);
		if (first != nullptr)
		{
			Remove(first);
		}
		return first != nullptr;
	}

	template<class T>
	bool DoublyLinkedList<T>::RemoveLast(const T& aValue)
	{
		auto last = FindLast(aValue);
		if (last != nullptr)
		{
			Remove(last);
		}
		return last != nullptr;

	}
}
