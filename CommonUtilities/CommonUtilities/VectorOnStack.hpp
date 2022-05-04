#pragma once
#include <initializer_list>
#include <cassert>


namespace CommonUtilities
{
	template<typename T, int SIZE, typename COUNT_TYPE = unsigned short, bool SAFEMODE = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack< T, SIZE, COUNT_TYPE, SAFEMODE>& aVectorOnStack);
		VectorOnStack(const std::initializer_list<T>& aInitList);

		~VectorOnStack();

		inline VectorOnStack< T, SIZE, COUNT_TYPE, SAFEMODE>& operator=(const VectorOnStack< T, SIZE, COUNT_TYPE, SAFEMODE>& aVectorOnStack);
		inline VectorOnStack< T, SIZE, COUNT_TYPE, SAFEMODE>& operator=(const std::initializer_list<T>& aInitList);

		inline const T& operator[](const COUNT_TYPE aIndex) const;
		inline T& operator[](const COUNT_TYPE aIndex);

		inline void Add(const T& aObject);
		inline void Insert(const COUNT_TYPE aIndex, const T& aObject);
		inline void RemoveCyclic(const T& aObject);
		inline void RemoveCyclicAtIndex(const COUNT_TYPE aItemNumber);
		inline void Clear();

		__forceinline int Size() const;
		__forceinline int MaxSize() const;

		inline T* Data();

	private:
		COUNT_TYPE mySize;
		T myElements[SIZE];
		void CheckIfOutsideBounds(int aIndex) const
		{
			assert(aIndex < mySize && -1 < aIndex && "[OUT OF BOUNDS] Index was outside of bounds");
		}
	};

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::VectorOnStack()
	{
		mySize = 0;
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::VectorOnStack(const VectorOnStack< T, SIZE, COUNT_TYPE, SAFEMODE>& aVectorOnStack)
	{
		mySize = 0;

		if (!SAFEMODE)
		{
			for (int i = 0; i < aVectorOnStack.Size(); i++)
			{
				std::memcpy(&myElements[i], &aVectorOnStack[i], sizeof(T));
				++mySize;
			}
		}
		else
		{
			for (int i = 0; i < aVectorOnStack.Size(); i++)
			{
				myElements[i] = aVectorOnStack[i];
				++mySize;
			}
		}
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::VectorOnStack(const std::initializer_list<T>& aInitList)
	{
		assert(aInitList.size() <= SIZE && "Size of initializer list was bigger than max size of VectorOnStack");
		mySize = aInitList.size();
		COUNT_TYPE i = 0;
		for (T e : aInitList)
		{
			myElements[i++] = e;
		}
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::~VectorOnStack()
	{
		mySize = 0;
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>& VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::operator=(const VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>& aVectorOnStack)
	{
		if (SAFEMODE)
		{
			mySize = aVectorOnStack.mySize;
			for (int i = 0; i < mySize; i++)
			{
				myElements[i] = aVectorOnStack.myElements[i];
			}
		}
		else
		{
			std::memcpy(&mySize, &(aVectorOnStack.mySize), sizeof(mySize));
			std::memcpy(myElements, aVectorOnStack.myElements, sizeof(T) * mySize);
		}
		return *this;
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>& VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::operator=(const std::initializer_list<T>& aInitList)
	{
		*this = VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>(aInitList);
		return *this;
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline const T& VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::operator[](const COUNT_TYPE aIndex) const
	{
		CheckIfOutsideBounds(aIndex);
		return myElements[aIndex];
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline T& VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::operator[](const COUNT_TYPE aIndex)
	{
		CheckIfOutsideBounds(aIndex);
		return myElements[aIndex];
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline void VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::Add(const T& aObject)
	{
		assert(mySize < SIZE && "[OUT OF BOUNDS] Cannot add another element to vector when size is more than or equal to max size of vector");
		myElements[mySize++] = aObject;
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline void VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::Insert(const COUNT_TYPE aIndex, const T& aObject)
	{
		assert(mySize < SIZE && "[OUT OF BOUNDS] Cannot add another element to vector when size is more than or equal to max size of vector");
		mySize++;
		CheckIfOutsideBounds(aIndex);

		if (SAFEMODE)
		{
			for (int i = mySize - 1; i > aIndex; i--)
			{
				myElements[i] = myElements[i - 1];
			}
			myElements[aIndex] = aObject;
		}
		else
		{
			memcpy(&myElements[aIndex + 1], &myElements[aIndex], sizeof(T) * (mySize - aIndex));

			std::memcpy(&myElements[aIndex], &aObject, sizeof(T));
		}
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline void VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::RemoveCyclic(const T& aObject)
	{
		for (int i = 0; i < mySize; i++)
		{
			if (myElements[i] == aObject)
			{
				myElements[i] = myElements[--mySize];
				return;
			}
		}
		assert(false && "Could not find object in VectorOnStack.");
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline void VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::RemoveCyclicAtIndex(const COUNT_TYPE aItemNumber)
	{
		CheckIfOutsideBounds(aItemNumber);

		--mySize;
		if (aItemNumber < mySize)
		{
			myElements[aItemNumber] = myElements[mySize];
		}
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline void VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::Clear()
	{
		mySize = 0;
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline int VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::Size() const
	{
		return mySize;
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline int VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::MaxSize() const
	{
		return SIZE;
	}

	template<typename T, int SIZE, typename COUNT_TYPE, bool SAFEMODE>
	inline T* VectorOnStack<T, SIZE, COUNT_TYPE, SAFEMODE>::Data()
	{
		return myElements;
	}


}