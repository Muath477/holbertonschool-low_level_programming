#include "main.h"

/**
 * _memset - fills the first n bytes of memory area s with byte b
 * @s: The memory area
 * @b: The byte to fill with
 * @n: The number of bytes to fill
 *
 * Return: A pointer to the memory area s
 */
char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i;

	i = 0;
	while (i < n)
	{
		s[i] = b;
		i++;
	}
	return (s);
}
