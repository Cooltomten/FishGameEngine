#pragma once
#include <cassert>
#include <vector>
namespace CommonUtilities
{
	template <class T>
	class MinHeap
	{
	public:
		int GetSize() const; //returnerar antal element i MinHeapen
		void Enqueue(const T& aElement); //lägger till elementet i MinHeapen
		const T& GetTop() const; //returnerar det största elementet i MinHeapen
		T Dequeue(); //tar bort det största elementet ur MinHeapen och returnerar det
	private:
		std::vector<T> myElements;
	};
	template<class T>
	inline int MinHeap<T>::GetSize() const
	{
		return myElements.size();
	}
	template<class T>
	inline void MinHeap<T>::Enqueue(const T& aElement)
	{
		int index = myElements.size();
		myElements.push_back(aElement);
		int parentIndex = (index - 1) / 2;
		while (myElements[index] < myElements[parentIndex] && 0 < index)
		{
			//swap parent and child
			myElements[index] = myElements[parentIndex];
			myElements[parentIndex] = aElement;

			//update index
			index = parentIndex;
			parentIndex = (index - 1) / 2;
		}
	}
	template<class T>
	inline const T& MinHeap<T>::GetTop() const
	{
		assert(0 < myElements.size() && "MinHeap was empty when trying to call GetTop()");
		return myElements[0];
	}
	template<class T>
	inline T MinHeap<T>::Dequeue()
	{
		assert(0 < myElements.size() && "MinHeap was empty when trying to call Dequeue()");
		T returnValue = myElements[0];
		myElements[0] = myElements.back();
		myElements.pop_back();

		int index = 0;

		bool shouldRun = 0 < myElements.size();
		int leftChildIndex = 0;
		int rightChildIndex = 0;
		while (shouldRun)
		{
			leftChildIndex = index * 2 + 1;
			rightChildIndex = index * 2 + 2;

			int checkIndex = 0;
			//make sure the indices are inside array
			if ((myElements.size() - 1) < rightChildIndex)
			{
				if ((myElements.size() - 1) < leftChildIndex)
				{
					shouldRun = false;
					break; // has no children
				}
				checkIndex = leftChildIndex;
			}
			//compare sizes
			else
			{
				if (myElements[rightChildIndex] < myElements[leftChildIndex])
				{
					//choose right child
					checkIndex = rightChildIndex;
				}
				else
				{
					//choose left child
					checkIndex = leftChildIndex;
				}
			}

			if (myElements[checkIndex] < myElements[index])
			{
				T temp = myElements[index];
				myElements[index] = myElements[checkIndex];
				myElements[checkIndex] = temp;
				index = checkIndex;
			}
			else
			{
				shouldRun = false;
				break; // has no children with lower value
			}
		}


		return returnValue;
	}
}