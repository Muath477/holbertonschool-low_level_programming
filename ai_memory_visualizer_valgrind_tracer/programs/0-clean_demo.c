#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * duplicate - allocates a heap copy of a string
 * @src: string to copy
 *
 * Return: pointer to the new heap block, or NULL on failure
 */
char *duplicate(const char *src)
{
	char *copy = malloc(strlen(src) + 1);

	if (copy == NULL)
		return (NULL);

	strcpy(copy, src);
	return (copy);
}

/**
 * sum_range - recursively adds every integer from 1 to n
 * @n: upper bound of the sum
 *
 * Return: the sum, or 0 if n <= 0
 */
int sum_range(int n)
{
	if (n <= 0)
		return (0);

	return (n + sum_range(n - 1));
}

int main(void)
{
	char *original = duplicate("holberton");
	char *alias = original;
	int total;

	if (original == NULL)
		return (1);

	printf("original: %s (%p)\n", original, (void *)original);
	printf("alias:    %s (%p)\n", alias, (void *)alias);

	alias[0] = 'H';
	printf("after mutation through alias, original: %s\n", original);

	total = sum_range(4);
	printf("sum_range(4) = %d\n", total);

	free(original);
	original = NULL;

	printf("original is now %p, alias is still %p (dangling)\n",
	       (void *)original, (void *)alias);

	return (0);
}
