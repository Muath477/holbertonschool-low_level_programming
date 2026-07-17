#include "main.h"
#include <stdlib.h>

/**
 * _strstr - locates a substring
 * @haystack: The string to search in
 * @needle: The substring to find
 *
 * Return: A pointer to the beginning of the located substring, or NULL
 */
char *_strstr(char *haystack, char *needle)
{
	int i;

	while (*haystack != '\0')
	{
		i = 0;
		while (needle[i] != '\0' && haystack[i] == needle[i])
			i++;
		if (needle[i] == '\0')
			return (haystack);
		haystack++;
	}
	if (*needle == '\0')
		return (haystack);
	return (NULL);
}
