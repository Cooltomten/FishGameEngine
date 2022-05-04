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
		//Skapar en tom k�
		Queue();

		~Queue();
		//Returnerar antal element i k�n
		int GetSize() const;
		//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
		//tom
		const T& GetFront() const;
		//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
		//tom
		T& GetFront();
		//L�gger in ett nytt element l�ngst bak i k�n
		void Enqueue(const T& aValue);
		//Tar bort elementet l�ngst fram i k�n och returnerar det. Kraschar med en
		//assert om k�n �r tom.
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