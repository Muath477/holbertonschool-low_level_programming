#include "main.h"

/**
 * print_diagonal - draws a diagonal line on the terminal
 * @n: The number of times the character \ should be printed
 *
 * Return: void
 */
void print_diagonal(int n)
{
	int i;
	int spaces;

	i = 0;
	while (i < n)
	{
		spaces = 0;
		while (spaces < i)
		{
			_putchar(' ');
			spaces++;
		}
		_putchar('\\');
		_putchar('\n');
		i++;
	}
	if (n <= 0)
		_putchar('\n');
}
