#include "main.h"

/**
 * print_square - prints a square using the character #
 * @size: The size of the square
 *
 * Return: void
 */
void print_square(int size)
{
	int row;
	int col;

	if (size <= 0)
	{
		_putchar('\n');
		return;
	}
	row = 0;
	while (row < size)
	{
		col = 0;
		while (col < size)
		{
			_putchar('#');
			col++;
		}
		_putchar('\n');
		row++;
	}
}
