#include "main.h"

/**
 * sqrt_helper - tries guesses one by one until it finds the square root
 * @n: the number we are looking for the square root of
 * @guess: the current guess
 *
 * Return: the square root if found, or -1 if there is none
 */
static int sqrt_helper(int n, int guess)
{
	if (guess * guess == n)
		return (guess);

	if (guess * guess > n)
		return (-1);

	return (sqrt_helper(n, guess + 1));
}

/**
 * _sqrt_recursion - returns the natural square root of a number, recursively
 * @n: the number to find the square root of
 *
 * Return: the natural square root of n, or -1 if it has none
 */
int _sqrt_recursion(int n)
{
	if (n < 0)
		return (-1);

	return (sqrt_helper(n, 0));
}
