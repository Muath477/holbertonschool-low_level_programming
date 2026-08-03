#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int *flag = malloc(sizeof(int));

	if (flag == NULL)
		return (1);

	if (*flag)
		printf("flag was set\n");
	else
		printf("flag was not set\n");

	free(flag);
	return (0);
}
