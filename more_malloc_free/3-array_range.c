#include "main.h"
#include <stdlib.h>

/**
 * array_range - creates an array of integers from min to max
 * @min: The minimum value, included
 * @max: The maximum value, included
 *
 * Return: A pointer to the new array, or NULL on failure
 */
int *array_range(int min, int max)
{
	int *array;
	int i;
	int count;

	if (min > max)
		return (NULL);

	count = max - min + 1;
	array = malloc(sizeof(int) * count);
	if (array == NULL)
		return (NULL);

	i = 0;
	while (i < count)
	{
		array[i] = min + i;
		i++;
	}
	return (array);
}
