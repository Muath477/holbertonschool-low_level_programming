#include "main.h"

/**
 * _pow_recursion - returns x raised to the power y, recursively
 * @x: the base
 * @y: the power
 *
 * Return: x to the power of y, or -1 if y is negative
 */
int _pow_recursion(int x, int y)
{
	if (y < 0)
		return (-1);

	if (y == 0)
		return (1);

	return (x * _pow_recursion(x, y - 1));
}
