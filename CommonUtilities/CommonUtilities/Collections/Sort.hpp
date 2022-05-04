#pragma once
#include <vector>
namespace CommonUtilities
{
	namespace
	{
		template <class T>
		void SelectionSort(std::vector<T>& aVector);
		template <class T>
		void BubbleSort(std::vector<T>& aVector);
		template <class T>
		void QuickSort(std::vector<T>& aVector);
		template <class T>
		void MergeSort(std::vector<T>& aVector);

		template<class T>
		void SelectionSort(std::vector<T>& aVector)
		{
			for (int i = 0; i < aVector.size(); i++)
			{
				int smallestIndex = i;
				for (int j = i; j < aVector.size(); j++)
				{
					if (aVector[j] < aVector[smallestIndex])
					{
						smallestIndex = j;
					}

				}
				T temp = aVector[i];
				aVector[i] = aVector[smallestIndex];
				aVector[smallestIndex] = temp;

			}
		}

		template<class T>
		void BubbleSort(std::vector<T>& aVector)
		{
			bool swapped = true;
			while (swapped)
			{
				swapped = false;
				for (int i = 1; i < aVector.size(); i++)
				{
					if (aVector[i] < aVector[i - 1])
					{
						swapped = true;
						T temp = aVector[i - 1];
						aVector[i - 1] = aVector[i];
						aVector[i] = temp;
					}
				}
			}
		}

		template<class T >
		int QuickSortPartition(std::vector<T>& aVector, int aStartIndex, int aEndIndex)
		{
			int size = aEndIndex - aStartIndex;
			if (size <= 4)
			{
				//insertion sort
				for (int i = aStartIndex + 1; i <= aEndIndex; i++)
				{
					for (int j = i - 1; j >= 0; j--)
					{
						if (aVector[j + 1] < aVector[j])
						{
							T temp = aVector[j];
							aVector[j] = aVector[j + 1];
							aVector[j + 1] = temp;
						}
						else
						{
							break;
						}
					}
				}
				return -1;
			}


			//Median of three
			T first = aVector[aStartIndex];
			T middle = aVector[aStartIndex + size / 2];
			T last = aVector[aEndIndex];
			//Sort the three
			T temp;
			if (last < middle)
			{
				temp = middle;
				middle = last;
				last = temp;
			}
			if (middle < first)
			{
				temp = first;
				first = middle;
				middle = temp;
				if (last < middle)
				{
					temp = middle;
					middle = last;
					last = temp;
				}
			}

			//put them back and swap pivot with last
			aVector[aStartIndex] = first;
			aVector[aStartIndex + size / 2] = last;
			aVector[aEndIndex] = middle;


			int i = aStartIndex - 1;

			for (int j = aStartIndex; j < aEndIndex; j++)
			{
				if (aVector[j] < aVector[aEndIndex])
				{
					i++;
					T temp = aVector[j];
					aVector[j] = aVector[i];
					aVector[i] = temp;
				}
			}

			int pivotIndex = i + 1;

			temp = aVector[pivotIndex];
			aVector[pivotIndex] = aVector[aEndIndex];
			aVector[aEndIndex] = aVector[pivotIndex];
			return pivotIndex;


		}
		template<class T>
		void QuickSortRecursion(std::vector<T>& aVector, int aStartIndex, int aEndIndex)
		{
			if (aStartIndex < aEndIndex)
			{
				int pivotIndex = QuickSortPartition(aVector, aStartIndex, aEndIndex);
				if (pivotIndex == -1)
				{
					return;
				}
				QuickSortRecursion(aVector, aStartIndex, pivotIndex - 1);
				QuickSortRecursion(aVector, pivotIndex + 1, aEndIndex);
			}
		}

		template<class T>
		void QuickSort(std::vector<T>& aVector)
		{
			QuickSortRecursion(aVector, 0, aVector.size() - 1);
		}

		template <class T >
		void MergeSortRecursion(std::vector<T>& aVector, int aStartIndex, int aEndIndex)
		{
			if (aStartIndex == aEndIndex)
			{
				return;
			}

			int size = (aEndIndex - aStartIndex);
			int middle = aStartIndex + size / 2;

			MergeSortRecursion(aVector, aStartIndex, middle); // left
			MergeSortRecursion(aVector, middle + 1, aEndIndex); // right


			T* merged = new T[size + 1];

			int leftOffset = 0;
			int rightOffset = 0;
			for (int i = 0; i < size + 1; i++)
			{
				int leftIndex = aStartIndex + leftOffset;
				int rightIndex = middle + 1 + rightOffset;
				if (aVector[rightIndex] < aVector[leftIndex])
				{
					merged[i] = aVector[rightIndex++];
				}
				else
				{
					merged[i] = aVector[leftIndex++];
				}


			}

			for (int i = 0; i < size + 1; i++)
			{
				aVector[aStartIndex + i] = merged[i];
			}

			delete[] merged;
		}

		template<class T>
		void MergeSort(std::vector<T>& aVector)
		{
			MergeSortRecursion(aVector, 0, aVector.size() - 1);
		}
	}
}