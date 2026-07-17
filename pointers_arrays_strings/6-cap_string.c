#include "main.h"

/**
 * is_separator - checks if a character is a word separator
 * @c: The character to check
 *
 * Return: 1 if c is a separator, 0 otherwise
 */
int is_separator(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == ',' || c == ';' ||
		c == '.' || c == '!' || c == '?' || c == '"' || c == '(' ||
		c == ')' || c == '{' || c == '}')
		return (1);
	return (0);
}

/**
 * cap_string - capitalizes all words of a string
 * @s: The string
 *
 * Return: A pointer to the string
 */
char *cap_string(char *s)
{
	int i;
	int new_word;

	new_word = 1;
	i = 0;
	while (s[i] != '\0')
	{
		if (new_word == 1 && s[i] >= 'a' && s[i] <= 'z')
			s[i] = s[i] - 32;
		if (is_separator(s[i]) == 1)
			new_word = 1;
		else
			new_word = 0;
		i++;
	}
	return (s);
}
