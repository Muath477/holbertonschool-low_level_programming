#include "main.h"

/**
 * _strspn - gets the length of a prefix substring
 * @s: The string
 * @accept: The bytes to accept
 *
 * Return: The number of bytes in the initial segment of s consisting
 * only of bytes from accept
 */
unsigned int _strspn(char *s, char *accept)
{
	unsigned int count;
	int j;
	int found;

	count = 0;
	while (*s != '\0')
	{
		found = 0;
		j = 0;
		while (accept[j] != '\0')
		{
			if (*s == accept[j])
			{
				found = 1;
				break;
			}
			j++;
		}
		if (found == 0)
			break;
		count++;
		s++;
	}
	return (count);
}

