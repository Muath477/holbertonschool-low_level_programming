#include <stdio.h>

/**
 * print_menu - Prints the calculator menu options
 *
 * Return: void
 */
void print_menu(void)
{
	printf("Simple Calculator\n");
	printf("1) Add\n");
	printf("2) Subtract\n");
	printf("3) Multiply\n");
	printf("4) Divide\n");
	printf("0) Quit\n");
}

/**
 * flush_input - Discards the rest of the current input line
 *
 * Description: Prevents an infinite loop when scanf fails to parse a
 * number, by consuming the characters it could not read.
 *
 * Return: void
 */
void flush_input(void)
{
	int c;

	while ((c = getchar()) != '\n' && c != EOF)
		;
}

/**
 * read_double - Prompts the user and reads one operand
 * @label: The text shown before the input (for example "A")
 * @value: Pointer to where the value read is stored
 *
 * Return: 1 on success, 0 if the input was not a valid number
 */
int read_double(const char *label, double *value)
{
	printf("%s: ", label);

	if (scanf("%lf", value) != 1)
	{
		flush_input();
		printf("Invalid number\n");
		return (0);
	}

	return (1);
}

/**
 * add - Adds two operands
 * @a: The first operand
 * @b: The second operand
 *
 * Return: The sum of @a and @b
 */
double add(double a, double b)
{
	return (a + b);
}

/**
 * subtract - Subtracts one operand from another
 * @a: The first operand
 * @b: The second operand
 *
 * Return: The difference of @a and @b
 */
double subtract(double a, double b)
{
	return (a - b);
}

/**
 * multiply - Multiplies two operands
 * @a: The first operand
 * @b: The second operand
 *
 * Return: The product of @a and @b
 */
double multiply(double a, double b)
{
	return (a * b);
}

/**
 * divide - Divides one operand by another
 * @a: The numerator
 * @b: The denominator, which must not be zero
 *
 * Description: The caller is responsible for checking that @b is not
 * zero before calling this function.
 *
 * Return: The quotient of @a and @b
 */
double divide(double a, double b)
{
	return (a / b);
}

/**
 * main - Entry point of the simple calculator program
 *
 * Description: Displays the menu, then loops asking for a choice.
 * Choices 1 to 4 read two operands and print the result of the
 * matching operation. Choice 0 quits.
 *
 * NUMBER MODEL: the calculator works with decimals. Operands are read
 * as double and every result is printed with the %g specifier, so whole
 * results print without a trailing ".0" (10 + 25 gives 35) while
 * fractional results keep their decimals (10 / 4 gives 2.5). Division
 * by zero is rejected before the division is performed.
 *
 * Return: 0 on success
 */
int main(void)
{
	int choice;
	double a, b;

	print_menu();

	while (1)
	{
		printf("Choice: ");

		if (scanf("%d", &choice) != 1)
		{
			flush_input();
			printf("Invalid choice\n");
			continue;
		}

		if (choice == 0)
		{
			printf("Bye!\n");
			break;
		}

		if (choice < 1 || choice > 4)
		{
			printf("Invalid choice\n");
			continue;
		}

		if (!read_double("A", &a) || !read_double("B", &b))
			continue;

		if (choice == 4 && b == 0)
		{
			printf("Error: division by zero\n");
			continue;
		}

		if (choice == 1)
			printf("Result: %g\n", add(a, b));
		else if (choice == 2)
			printf("Result: %g\n", subtract(a, b));
		else if (choice == 3)
			printf("Result: %g\n", multiply(a, b));
		else
			printf("Result: %g\n", divide(a, b));
	}

	return (0);
}
