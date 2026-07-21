#include "main.h"
#include <stdlib.h>

/**
 * _strdup - returns a pointer to a newly allocated copy of a string
 * @str: The string to duplicate
 *
 * Return: A pointer to the duplicated string, or NULL on failure
 */
char *_strdup(char *str)
{
	char *copy;
	unsigned int length;
	unsigned int i;

	if (str == NULL)
		return (NULL);

	length = 0;
	while (str[length] != '\0')
		length++;

	copy = malloc(sizeof(char) * (length + 1));
	if (copy == NULL)
		return (NULL);

	i = 0;
	while (i <= length)
	{
		copy[i] = str[i];
		i++;
	}
	return (copy);
}
