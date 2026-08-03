#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	size_t size = 5 * 1024 * 1024;
	int *big = malloc(size);

	if (big == NULL)
		return (1);

	big[0] = 42;
	printf("big[0] before free: %d\n", big[0]);

	free(big);

	big[0] = 1337;
	printf("big[0] after free: %d\n", big[0]);

	return (0);
}
