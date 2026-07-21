#include "main.h"
#include <stdlib.h>

/**
 * create_array - creates an array of chars and initializes it with a
 * specific char
 * @size: The size of the array
 * @c: The char to initialize the array with
 *
 * Return: A pointer to the array, or NULL if it fails or size is 0
 */
char *create_array(unsigned int size, char c)
{
	char *array;
	unsigned int i;

	if (size == 0)
		return (NULL);

	array = malloc(sizeof(char) * size);
	if (array == NULL)
		return (NULL);

	i = 0;
	while (i < size)
	{
		array[i] = c;
		i++;
	}
	return (array);
}
