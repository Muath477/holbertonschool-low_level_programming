#include "main.h"

/**
 * print_triangle - prints a triangle using the character #
 * @size: The size of the triangle
 *
 * Return: void
 */
void print_triangle(int size)
{
	int row;
	int spaces;
	int hashes;

	if (size <= 0)
	{
		_putchar('\n');
		return;
	}
	row = 1;
	while (row <= size)
	{
		spaces = 0;
		while (spaces < size - row)
		{
			_putchar(' ');
			spaces++;
		}
		hashes = 0;
		while (hashes < row)
		{
			_putchar('#');
			hashes++;
		}
		_putchar('\n');
		row++;
	}
}
