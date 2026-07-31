#include <stdio.h>
#include "main.h"

/**
 * only_digits - checks whether a string contains only digit characters
 * @s: string to check
 *
 * Return: 1 if s contains only digits, 0 otherwise
 */
int only_digits(char *s)
{
	int i;

	for (i = 0; s[i]; i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
	}

	return (1);
}

/**
 * main - adds positive numbers passed as arguments
 * @argc: number of arguments passed to the program
 * @argv: array of arguments
 *
 * Return: 0 on success, 1 if an argument is not a valid number
 */
int main(int argc, char *argv[])
{
	int i, j, num, sum;

	sum = 0;

	for (i = 1; i < argc; i++)
	{
		if (!only_digits(argv[i]))
		{
			printf("Error\n");
			return (1);
		}

		num = 0;
		for (j = 0; argv[i][j]; j++)
			num = num * 10 + (argv[i][j] - '0');

		sum += num;
	}

	printf("%d\n", sum);

	return (0);
}
