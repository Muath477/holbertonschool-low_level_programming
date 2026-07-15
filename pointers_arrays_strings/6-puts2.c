#include "main.h"

/**
 * puts2 - prints every other character of a string, starting with the
 * first, followed by a new line
 * @str: A pointer to the string
 *
 * Return: void
 */
void puts2(char *str)
{
	int length;
	int i;

	length = 0;
	while (str[length] != '\0')
		length++;

	i = 0;
	while (i < length)
	{
		_putchar(str[i]);
		i += 2;
	}
	_putchar('\n');
}
