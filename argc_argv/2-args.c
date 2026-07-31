#include <stdio.h>
#include "main.h"

/**
 * main - prints all arguments passed to the program, one per line
 * @argc: number of arguments passed to the program
 * @argv: array of arguments
 *
 * Return: always 0
 */
int main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < argc; i++)
		printf("%s\n", argv[i]);

	return (0);
}
