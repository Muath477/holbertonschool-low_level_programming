#include "function_pointers.h"

/**
 * int_index - searches for an integer in an array using a
 * comparison function
 * @array: array to search
 * @size: number of elements in the array
 * @cmp: pointer to the comparison function to use
 *
 * Return: index of the first element for which cmp does not
 * return 0, otherwise -1. If size <= 0, returns -1.
 */
int int_index(int *array, int size, int (*cmp)(int))
{
	int i;

	if (array == NULL || cmp == NULL || size <= 0)
		return (-1);

	for (i = 0; i < size; i++)
	{
		if (cmp(array[i]) != 0)
			return (i);
	}

	return (-1);
}
