#include <stdio.h>
#include <stdlib.h>
#include "3-calc.h"

/**
 * main - performs a simple operation from command line arguments
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, exits with status 98, 99, or 100 on error
 */
int main(int argc, char *argv[])
{
	int a, b;
	int (*f)(int, int);

	if (argc != 4)
	{
		printf("Error\n");
		exit(98);
	}

	f = get_op_func(argv[2]);
	if (f == NULL)
	{
		printf("Error\n");
		exit(99);
	}

	a = atoi(argv[1]);
	b = atoi(argv[3]);

	if (b == 0 && (argv[2][0] == '/' || argv[2][0] == '%'))
	{
		printf("Error\n");
		exit(100);
	}

	printf("%d\n", f(a, b));

	return (0);
}
