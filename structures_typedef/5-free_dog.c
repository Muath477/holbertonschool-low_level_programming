#include "dog.h"
#include <stdlib.h>

/**
 * free_dog - frees a dog created by new_dog
 * @d: A pointer to the dog to free
 *
 * Description: Frees the name and owner strings before freeing the
 * struct itself.
 *
 * Return: void
 */
void free_dog(dog_t *d)
{
	if (d == NULL)
		return;

	free(d->name);
	free(d->owner);
	free(d);
}

