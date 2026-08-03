#include <stdio.h>
#include <stdlib.h>

/**
 * build_message - allocates and fills a greeting on the heap
 * @name: name to greet
 *
 * Return: pointer to a heap-allocated string, or NULL on failure
 */
char *build_message(const char *name)
{
	char *msg = malloc(64);

	if (msg == NULL)
		return (NULL);

	sprintf(msg, "Hello, %s!", name);
	return (msg);
}

int main(void)
{
	char *greeting = build_message("Valgrind");

	if (greeting == NULL)
		return (1);

	printf("%s\n", greeting);

	return (0);
}
