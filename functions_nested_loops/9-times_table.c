#include "main.h"

/**
 * times_table - prints the 9 times table, starting with 0
 *
 * Return: void
 */
void times_table(void)
{
	int n;
	int m;
	int p;

	n = 0;
	while (n <= 9)
	{
		m = 0;
		while (m <= 9)
		{
			p = n * m;
			if (m == 0)
				_putchar(p + '0');
			else
			{
				_putchar(',');
				_putchar(' ');
				if (p < 10)
					_putchar(' ');
				else
					_putchar((p / 10) + '0');
				_putchar((p % 10) + '0');
			}
			m++;
		}
		_putchar('\n');
		n++;
	}
}
