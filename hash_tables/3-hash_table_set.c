#include <string.h>
#include "hash_tables.h"

/**
 * update_node - looks up key in a bucket and updates its value if found
 * @bucket: head of the bucket's linked list
 * @key: the key to look for
 * @value_copy: the already-duplicated value to store on a match
 *
 * Return: 1 if key was found and updated, 0 otherwise
 */
int update_node(hash_node_t *bucket, const char *key, char *value_copy)
{
	while (bucket != NULL)
	{
		if (strcmp(bucket->key, key) == 0)
		{
			free(bucket->value);
			bucket->value = value_copy;
			return (1);
		}
		bucket = bucket->next;
	}

	return (0);
}

/**
 * hash_table_set - adds or updates an element in the hash table
 * @ht: the hash table
 * @key: the key, can not be an empty string
 * @value: the value associated with the key, is duplicated
 *
 * Return: 1 if it succeeded, 0 otherwise
 */
int hash_table_set(hash_table_t *ht, const char *key, const char *value)
{
	unsigned long int index;
	hash_node_t *new_node;
	char *value_copy;

	if (ht == NULL || key == NULL || *key == '\0' || value == NULL)
		return (0);

	value_copy = strdup(value);
	if (value_copy == NULL)
		return (0);

	index = key_index((const unsigned char *)key, ht->size);

	if (update_node(ht->array[index], key, value_copy))
		return (1);

	new_node = malloc(sizeof(hash_node_t));
	if (new_node == NULL)
	{
		free(value_copy);
		return (0);
	}

	new_node->key = strdup(key);
	if (new_node->key == NULL)
	{
		free(value_copy);
		free(new_node);
		return (0);
	}

	new_node->value = value_copy;
	new_node->next = ht->array[index];
	ht->array[index] = new_node;

	return (1);
}
