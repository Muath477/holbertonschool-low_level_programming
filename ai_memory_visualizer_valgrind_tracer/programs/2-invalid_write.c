#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int *arr = malloc(5 * sizeof(int));
	int i;

	if (arr == NULL)
		return (1);

	for (i = 0; i <= 5; i++)
		arr[i] = i * 2;

	for (i = 0; i < 5; i++)
		printf("%d\n", arr[i]);

	free(arr);
	return (0);
}
