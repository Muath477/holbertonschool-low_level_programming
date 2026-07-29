#include <stddef.h>
#include "3-calc.h"

/**
 * get_op_func - selects the correct function for a given operator
 * @s: operator as a string
 *
 * Return: pointer to the function matching the operator, or
 * NULL if no match is found
 */
int (*get_op_func(char *s))(int, int)
{
	op_t ops[] = {
		{"+", op_add},
		{"-", op_sub},
		{"*", op_mul},
		{"/", op_div},
		{"%", op_mod},
		{NULL, NULL}
	};
	int i;

	i = 0;
	while (ops[i].op != NULL)
	{
		if (ops[i].op[0] == s[0])
			return (ops[i].f);
		i++;
	}

	return (NULL);
}
