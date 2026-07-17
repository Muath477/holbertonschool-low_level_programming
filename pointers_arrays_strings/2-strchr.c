#include "main.h"
#include <stdlib.h>

/**
 * _strchr - locates a character in a string
 * @s: The string
 * @c: The character to locate
 *
 * Return: A pointer to the first occurrence of c, or NULL if not found
 */
char *_strchr(char *s, char c)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (s + i);
		i++;
	}
	if (c == '\0')
		return (s + i);
	return (NULL);
}
