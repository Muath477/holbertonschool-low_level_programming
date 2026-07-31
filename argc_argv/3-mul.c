#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/**
 * main - multiplies two numbers passed as arguments
 * @argc: number of arguments passed to the program
 * @argv: array of arguments
 *
 * Return: 0 on success, 1 if the number of arguments is wrong
 */
int main(int argc, char *argv[])
{
	int result;

	if (argc != 3)
	{
		printf("Error\n");
		return (1);
	}

	result = atoi(argv[1]) * atoi(argv[2]);
	printf("%d\n", result);

	return (0);
}
