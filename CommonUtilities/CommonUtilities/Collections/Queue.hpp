#pragma once
#include <cassert>
#include "../UtilityFunctions.hpp"
#define SizeAssert	assert(mySize > 0 && "Queue is empty")
namespace CommonUtilities
{
	template <class T>
	class Queue
	{
	public:
		//Skapar en tom kö
		Queue();

		~Queue();
		//Returnerar antal element i kön
		int GetSize() const;
		//Returnerar elementet längst fram i kön. Kraschar med en assert om kön är
		//tom
		const T& GetFront() const;
		//Returnerar elementet längst fram i kön. Kraschar med en assert om kön är
		//tom
		T& GetFront();
		//Lägger in ett nytt element längst bak i kön
		void Enqueue(const T& aValue);
		//Tar bort elementet längst fram i kön och returnerar det. Kraschar med en
		//assert om kön är tom.
		T Dequeue();

	private:
		void Grow();
		int GetLast();
		T* myArray;
		int mySize;
		int myReservedSize;
		int myFirst;
	};
	template<class T>
	inline Queue<T>::Queue()
	{
		myReservedSize = 16;
		myArray = new T[myReservedSize];
		myFirst = 0;
		mySize = 0;
	}
	template<class T>
	inline Queue<T>::~Queue()
	{
		delete[] myArray;
	}
	template<class T>
	inline int Queue<T>::GetSize() const
	{
		return mySize;
	}
	template<class T>
	inline const T& Queue<T>::GetFront() const
	{
		SizeAssert;
		return myArray[myFirst];
	}
	template<class T>
	inline T& Queue<T>::GetFront()
	{
		SizeAssert;
		return myArray[myFirst];
	}
	template<class T>
	inline void Queue<T>::Enqueue(const T& aValue)
	{
		mySize++;

		if (GetLast() == myReservedSize)
		{
			Grow();
		}

		myArray[GetLast()] = aValue;
		 
		
	}
	template<class T>
	inline T Queue<T>::Dequeue()
	{
		SizeAssert;
		mySize--;

		T returnValue = myArray[myFirst];
		myFirst++;
		if (myFirst == myReservedSize)
		{
			myFirst = 0;
		}

		return returnValue;
	}
	template<class T>
	inline void Queue<T>::Grow()
	{
		myReservedSize *= 2;
		T* newArray = new T[myReservedSize];
		for (int i = 0; i < myReservedSize; i++)
		{
			int index = i + myFirst;
			if (index >= myReservedSize)
			{
				index -= myReservedSize;
			}
			newArray[i] = myArray[index];
		}
		delete[] myArray;
		myArray = newArray;
		myFirst = 0;
	}
	template<class T>
	inline int Queue<T>::GetLast()
	{
		int last = myFirst + mySize - 1;
		if (last >= myReservedSize)
		{
			last -= myReservedSize;
		}
		if (last < 0)
		{
			last = 0;
		}
		return last;
	}
}