#pragma once
#include <cassert>
#define SizeAssert	assert(mySize > 0 && "Stack is empty")
namespace CommonUtilities
{
	template <class T>
	class Stack
	{
	public:
		//Skapar en tom stack
		Stack();

		~Stack();

		//Returnerar antal element i stacken
		int GetSize() const;
		//Returnerar det �versta elementet i stacken. Kraschar med en assert om
		//stacken �r tom.
		const T& GetTop() const;
		//Returnerar det �versta elementet i stacken. Kraschar med en assert om
		//stacken �r tom.
		T& GetTop();
		//L�gger in ett nytt element �verst p� stacken
		void Push(const T& aValue);
		//Tar bort det �versta elementet fr�n stacken och returnerar det. Kraschar
		//med en assert om stacken �r tom.
		T Pop();
	private:
		void Grow();
		int GetTopIndex() const;
		T* myArray;
		int mySize;
		int myReservedSize;
	};
	template<class T>
	inline Stack<T>::Stack()
	{
		mySize = 0;
		myReservedSize = 16;
		myArray = new T[myReservedSize];
	}
	template<class T>
	inline Stack<T>::~Stack()
	{
		delete[] myArray;
	}
	template<class T>
	inline int Stack<T>::GetSize() const
	{
		return mySize;
	}
	template<class T>
	inline const T& Stack<T>::GetTop() const
	{
		SizeAssert;
		return  myArray[GetTopIndex()];
	}
	template<class T>
	inline T& Stack<T>::GetTop()
	{
		SizeAssert;
		return myArray[GetTopIndex()];
	}
	template<class T>
	inline void Stack<T>::Push(const T& aValue)
	{
		mySize++;
		if (GetTopIndex() == -1)
		{
			Grow();
		}
		myArray[GetTopIndex()] = aValue;
	}
	template<class T>
	inline T Stack<T>::Pop()
	{
		SizeAssert;
		return myArray[(myReservedSize - mySize--)];
	}
	template<class T>
	inline void Stack<T>::Grow()
	{
		myReservedSize *= 2;
		T* newArray = new T[myReservedSize];
		for (int i = myReservedSize - 1; i >= myReservedSize - mySize; i--)
		{
			newArray[i] = myArray[i - mySize + 1];
		}
		delete[] myArray;
		myArray = newArray;


	}
	template<class T>
	inline int Stack<T>::GetTopIndex() const 
	{
		return (myReservedSize - mySize);
	}
}
#undef SizeAssert