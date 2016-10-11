#pragma once
#include <vector>

/**
 * Simple insertion sort (drive).
 */
template <class Comparable>
void insertionSort(std::vector<Comparable> & a)
{
	insertionSort(a, 0, a.size()-1);
}
/**
 * Simple insertion sort.
 * a is an array of Comparable items.
 * left is the left-most index of the subarray.
 * right is the right-most index of the subarray.
*/
template <class Comparable>
void insertionSort(std::vector<Comparable> & a, size_t left, size_t right)
{
	for (size_t i = left+1; i <= right; ++i)
	{
		Comparable temp = std::move(a[i]);
		size_t j;
		for (j = i; j > left && temp < a[j - 1]; --j)
			a[j] = std::move(a[j - 1]);
		a[j] = std::move(temp);
	}
}

/**
 * Mergesort algorithm (driver).
 */
template <typename Comparable>
void mergeSort(std::vector<Comparable> & a)
{
	std::vector<Comparable> tmpArray(a.size());
	mergeSort(a, tmpArray, 0, a.size() - 1);
}
/**
 * Internal method that makes recursive calls.
 * a is an array of Comparable items.
 * tmpArray is an array to place the merged result.
 * left is the left-most index of the subarray.
 * right is the right-most index of the subarray.
 */
template <typename Comparable>
void mergeSort(std::vector<Comparable> & a, std::vector<Comparable> & tmpArray, size_t left, size_t right)
{
	if (left < right)
	{
		size_t center = (left + right) / 2;
		mergeSort(a, tmpArray, left, center);
		mergeSort(a, tmpArray, center + 1, right);
		merge(a, tmpArray, left, center + 1, right);
	}
}

/**
 * Internal method that merges two sorted halves of a subarray.
 * a is an array of Comparable items.
 * tmpArray is an array to place the merged result.
 * leftPos is the left-most index of the subarray.
 * rightPos is the index of the start of the second half.
 * rightEnd is the right-most index of the subarray.
 */
template <typename Comparable>
void merge(std::vector<Comparable> & a, std::vector<Comparable> & tmpArray, size_t leftPos, size_t rightPos, size_t rightEnd)
{
	size_t leftEnd = rightPos - 1;
	size_t tmpPos = leftPos;
	size_t numElements = rightEnd - leftPos + 1;

	// Main loop
	while (leftPos <= leftEnd && rightPos <= rightEnd)
	{
		if (a[leftPos] <= a[rightPos])
		{
			tmpArray[tmpPos++] = std::move(a[leftPos++]);
		}
		else
		{
			tmpArray[tmpPos++] = std::move(a[rightPos++]);
		}
	}
	while (leftPos <= leftEnd) // Copy rest of first half
	{
		tmpArray[tmpPos++] = std::move(a[leftPos++]);
	}
	while (rightPos <= rightEnd) // Copy rest of right half
	{
		tmpArray[tmpPos++] = std::move(a[rightPos++]);
	}

	// Copy tmpArray back
	for (size_t i = 0; i < numElements; ++i, --rightEnd) { a[rightEnd] = std::move(tmpArray[rightEnd]); }
}


#define TIM_SWITCHOVER 48

/**
 * Timsort algorithm (driver).
 */
template <typename Comparable>
void timSort(std::vector<Comparable> & a)
{
	std::vector<Comparable> tmpArray(a.size());
	timSort(a, tmpArray, 0, a.size() - 1);
}
/**
 * Internal method that makes recursive calls.
 * a is an array of Comparable items.
 * tmpArray is an array to place the merged result.
 * left is the left-most index of the subarray.
 * right is the right-most index of the subarray.
 */
template <typename Comparable>
void timSort(std::vector<Comparable> & a, std::vector<Comparable> & tmpArray, size_t left, size_t right)
{
	if (left + TIM_SWITCHOVER < right)
	{
		size_t center = (left + right) / 2;
		timSort(a, tmpArray, left, center);
		timSort(a, tmpArray, center + 1, right);
		merge(a, tmpArray, left, center + 1, right);
	}
	else
	{
		insertionSort(a, left, right);
	}
}
