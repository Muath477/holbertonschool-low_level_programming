#include "variadic_functions.h"
#include <stdio.h>

/**
 * print_char - prints a char argument
 * @args: variadic argument list
 *
 * Return: nothing
 */
void print_char(va_list args)
{
	printf("%c", va_arg(args, int));
}

/**
 * print_int - prints an int argument
 * @args: variadic argument list
 *
 * Return: nothing
 */
void print_int(va_list args)
{
	printf("%d", va_arg(args, int));
}

/**
 * print_float - prints a float argument
 * @args: variadic argument list
 *
 * Return: nothing
 */
void print_float(va_list args)
{
	printf("%f", va_arg(args, double));
}

/**
 * print_string - prints a char * argument
 * @args: variadic argument list
 *
 * Return: nothing
 */
void print_string(va_list args)
{
	char *str;

	str = va_arg(args, char *);
	if (str == NULL)
		str = "(nil)";
	printf("%s", str);
}

/**
 * print_all - prints anything, according to a format string
 * @format: list of types of the arguments passed to the function
 *
 * Return: nothing
 */
void print_all(const char * const format, ...)
{
	va_list args;
	unsigned int i, j;
	char *sep = "";
	printer_t printers[] = {
		{'c', print_char},
		{'i', print_int},
		{'f', print_float},
		{'s', print_string},
		{0, NULL}
	};

	va_start(args, format);

	i = 0;
	while (format != NULL && format[i] != '\0')
	{
		j = 0;
		while (printers[j].f != NULL)
		{
			if (printers[j].t == format[i])
			{
				printf("%s", sep);
				printers[j].f(args);
				sep = ", ";
				break;
			}
			j++;
		}
		i++;
	}

	printf("\n");
	va_end(args);
}
