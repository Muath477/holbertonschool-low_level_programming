#include "dog.h"
#include <stdlib.h>

/**
 * _strlen - returns the length of a string
 * @s: The string
 *
 * Return: The length of s
 */
int _strlen(char *s)
{
	int len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

/**
 * _strcopy - copies a string into a buffer
 * @dest: The destination buffer
 * @src: The source string
 *
 * Return: A pointer to dest
 */
char *_strcopy(char *dest, char *src)
{
	int i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/**
 * new_dog - creates a new dog
 * @name: The name of the dog
 * @age: The age of the dog
 * @owner: The owner of the dog
 *
 * Description: Stores a copy of name and owner in newly allocated memory.
 *
 * Return: A pointer to the new dog, or NULL on failure
 */
dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *dog;

	if (name == NULL || owner == NULL)
		return (NULL);

	dog = malloc(sizeof(dog_t));
	if (dog == NULL)
		return (NULL);

	dog->name = malloc(sizeof(char) * (_strlen(name) + 1));
	if (dog->name == NULL)
	{
		free(dog);
		return (NULL);
	}

	dog->owner = malloc(sizeof(char) * (_strlen(owner) + 1));
	if (dog->owner == NULL)
	{
		free(dog->name);
		free(dog);
		return (NULL);
	}

	_strcopy(dog->name, name);
	_strcopy(dog->owner, owner);
	dog->age = age;
	return (dog);
}
