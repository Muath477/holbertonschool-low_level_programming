#include "main.h"

/**
 * _strncpy - copies at most n characters from src to dest
 * @dest: The destination buffer
 * @src: The source string
 * @n: The number of bytes to copy
 *
 * Return: A pointer to dest
 */
char *_strncpy(char *dest, char *src, int n)
{
	int i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
