#pragma once

#include <vector>

namespace CommonUtilities
{
	namespace details
	{
		template<typename T>
		void MergeRecursive(std::vector<T>& aVector, std::vector<T>& aAuxiliary, int aLeft, int aRight);

		template<typename T>
		void Merge(std::vector<T>& aVector, std::vector<T>& aAuxiliary, int aLow, int aMiddle, int aHigh);

		template<typename T>
		void QuickRecursive(std::vector<T>& aVector, int aLow, int aHigh);

		template<typename T>
		inline void MergeRecursive(std::vector<T>& aVector, std::vector<T>& aAuxiliary, int aLeft, int aRight)
		{
			if (!(aRight > aLeft))
				return;

			int middle = aLeft + (aRight - aLeft) / 2;

			MergeRecursive(aVector, aAuxiliary, aLeft, middle);
			MergeRecursive(aVector, aAuxiliary, middle + 1, aRight);

			Merge(aVector, aAuxiliary, aLeft, middle, aRight);
		}

		template<typename T>
		inline void Merge(std::vector<T>& aVector, std::vector<T>& aAuxiliary, int aLow, int aMiddle, int aHigh)
		{
			int i = aLow, j = aMiddle + 1, k = 0;
			while (i <= aMiddle && j <= aHigh)
			{
				aAuxiliary[k++] = aVector[i] < aVector[j] 
					? aVector[i++] : aVector[j++];
			}

			while (i <= aMiddle)
			{
				aAuxiliary[k++] = aVector[i++];
			}

			while (j <= aHigh)
			{
				aAuxiliary[k++] = aVector[j++];
			}

			int count = (aHigh - aLow + 1);
			for (int l = 0; l < count; ++l)
			{
				aVector[aLow + l] = aAuxiliary[l];
			}
		}

		template<typename T>
		inline void QuickRecursive(std::vector<T>& aVector, int aLow, int aHigh)
		{
			if (!(aHigh > aLow))
				return;

			T& pivot = aVector[aLow];
			int i = aLow, j = aHigh;

			while (true)
			{
				while ((aVector[i] < pivot || !(pivot < aVector[i])) && i < aHigh) 
					++i;
				
				while (pivot < aVector[j])
					--j;

				if (i < j)
				{
					std::swap(aVector[i], aVector[j]);
				}
				else
				{
					break;
				}
			}

			std::swap(pivot, aVector[j]);

			QuickRecursive(aVector, aLow, j - 1);
			QuickRecursive(aVector, j + 1, aHigh);
		}
	}

	template<typename T>
	void SelectionSort(std::vector<T>& aVector);

	template<typename T>
	void BubbleSort(std::vector<T>& aVector);

	template<typename T>
	void QuickSort(std::vector<T>& aVector);

	template<typename T>
	void MergeSort(std::vector<T>& aVector);

	template<typename T>
	inline void SelectionSort(std::vector<T>& aVector)
	{
		for (int i = 0; i < (int)aVector.size() - 1; ++i)
		{
			int minIndex = i;
			for (int j = i + 1; j < (int)aVector.size(); ++j)
			{
				if (aVector[j] < aVector[minIndex])
				{
					minIndex = j;
				}
			}

			if (minIndex != i)
			{
				std::swap(aVector[minIndex], aVector[i]);
			}
		}
	}

	template<typename T>
	inline void BubbleSort(std::vector<T>& aVector)
	{
		bool swapped = false;
		for (int i = 0; i < (int)aVector.size() - 1; ++i)
		{
			swapped = false;
			for (int j = 0; j < (int)aVector.size() - i - 1; ++j)
			{
				if (aVector[j + 1] < aVector[j])
				{
					std::swap(aVector[j], aVector[j + 1]);
					swapped = true;
				}
			}

			if (!swapped)
				break;
		}
	}

	template<typename T>
	inline void QuickSort(std::vector<T>& aVector)
	{
		details::QuickRecursive(aVector, 0, (int)aVector.size() - 1);
	}

	template<typename T>
	inline void MergeSort(std::vector<T>& aVector)
	{
		static thread_local std::vector<T> aux(aVector.size()); // auxiliary storage for merge because we don't do inplace
		details::MergeRecursive(aVector, aux, 0, (int)aVector.size() - 1);
	}
}