#include "main.h"

/**
 * puts_half - prints the second half of a string, followed by a new line
 * @str: A pointer to the string
 *
 * Return: void
 */
void puts_half(char *str)
{
	int length;
	int i;

	length = 0;
	while (str[length] != '\0')
		length++;

	i = (length + 1) / 2;
	while (i < length)
	{
		_putchar(str[i]);
		i++;
	}
	_putchar('\n');
}
