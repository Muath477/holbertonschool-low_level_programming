#include "main.h"

/**
 * check_divisors - checks recursively if n has a divisor starting at i
 * @n: the number to check
 * @i: the current divisor being tested
 *
 * Return: 1 if n is prime, 0 otherwise
 */
int check_divisors(int n, int i)
{
	if (n <= 1)
		return (0);

	if (i * i > n)
		return (1);

	if (n % i == 0)
		return (0);

	return (check_divisors(n, i + 1));
}

/**
 * is_prime_number - checks if an integer is a prime number, recursively
 * @n: the number to check
 *
 * Return: 1 if n is prime, 0 otherwise
 */
int is_prime_number(int n)
{
	return (check_divisors(n, 2));
}
