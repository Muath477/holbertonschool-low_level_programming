#include "main.h"

/**
 * rev_string - reverses a string in place
 * @s: A pointer to the string to reverse
 *
 * Return: void
 */
void rev_string(char *s)
{
	int start;
	int end;
	char temp;

	end = 0;
	while (s[end] != '\0')
		end++;
	end--;

	start = 0;
	while (start < end)
	{
		temp = s[start];
		s[start] = s[end];
		s[end] = temp;
		start++;
		end--;
	}
}
