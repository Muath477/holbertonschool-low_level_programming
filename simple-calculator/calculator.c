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
 * main - Entry point of the simple calculator program
 *
 * Description: Displays a menu and reads the user's choice.
 * For now, only option 0 (Quit) is handled.
 *
 * Return: 0 on success
 */
int main(void)
{
	int choice;
	print_menu();
	printf("Choice: ");
	if (scanf("%d", &choice) != 1)
	{
		printf("Invalid input\n");
		return (1);
	}
	if (choice == 0)
	{
		printf("Bye!\n");
		return (0);
	}
	printf("Not implemented yet\n");
	return (0);
}


