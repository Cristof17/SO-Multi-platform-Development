#include <stdio.h>
#include "hash.h"
#include <stdint.h>
#include <stdlib.h>

#define BUFFSIZE 20000
#define ADD 0
#define REMOVE 1
#define FIND 2
#define PRINT_BUCKET 3
#define PRINT 4
#define RESIZE 5
#define RESIZE_DOUBLE 6
#define RESIZE_HALF 7
#define CLEAR 8
#define DEFAULT 9

struct node {
	char *cuvant;
	struct node *next;
};

struct bucket {
	struct node *top;
};

struct hashtable {
	int size;
	struct bucket **buckets;
};

void add(struct hashtable *hashtable, char *word)
{
	unsigned int hash_code;
	struct bucket *target_bucket;
	struct node *new_node;

	if (hashtable == NULL) {
		/*
		 * TODO
		 * HashTable is null error
		 */
	}
	hash_code = hash(word, hashtable->size);
	target_bucket = hashtable->buckets[hash_code];
	new_node = (struct node *)malloc(1 * sizeof(struct node));
	new_node->cuvant = word;
	new_node->next = NULL;
	if (target_bucket == NULL) {
		target_bucket = (struct bucket *)
					malloc(1 * sizeof(struct bucket));
		target_bucket->top = new_node;
	} else {
		struct node *iterator = target_bucket->top;

		while (iterator != NULL)
			iterator = iterator->next;
		iterator->next = new_node;
	}
}

int get_operation_code(char *operation)
{
	if (strcmp(operation, "add") == 0)
		return ADD;
	else if (strcmp(operation, "remove") == 0)
		return REMOVE;
	else if (strcmp(operation, "find") == 0)
		return FIND;
	else if (strcmp(operation, "clear") == 0)
		return CLEAR;
	else if (strcmp(operation, "print_bucket") == 0)
		return PRINT_BUCKET;
	else if (strcmp(operation, "print") == 0)
		return PRINT;
	else if (strcmp(operation, "resize") == 0)
		return RESIZE;
	else
		return DEFAULT;
}

int main(int argc, char **argv)
{
	int i;
	uint32_t lungime;
	struct hashtable *hashtable;

	/* reading argc to know if there is any input files */
	if (argc == 1) {
		printf("./tema1 <numberOfBuckets\n");
		/* TODO
		 * READ FROM STDIN
		 */
		return -1;
	}
	lungime = (uint32_t)argv[1];
	if (argc == 2) {
		/* TODO
		 * READ FROM STDIN
		 */
		lungime = (uint32_t)argv[1];
		hashtable = (struct hashtable *)
					malloc(1 * sizeof(struct hashtable));
		hashtable->buckets = (struct bucket **)
				malloc(lungime * sizeof(struct bucket *));
		hashtable->size = lungime;
	} else {
		for (i = 2; i < argc; ++i) {
			FILE *file = fopen(argv[i], "r+");
			char *buffer;
			char *token;
			int opcode;
			/*
			 * Read from file line by line
			 */
			buffer = (char *)malloc(BUFFSIZE * sizeof(char));
			fgets(buffer, BUFFSIZE, file);
			token = (char *)strtok(buffer, " ");
			opcode = get_operation_code(token);
			switch (opcode) {
			case ADD:
			{
				char *argument;

				argument = (char *)strtok(NULL, " ");
				printf("Add argument is %s\n",argument);
				add(hashtable, argument);
				break;
			}
			case PRINT:
				printf("Printing hashtable\n");
			case FIND:
				printf("Finding word\n");
			case REMOVE:
				printf("Removing word\n");
			case CLEAR:
				printf("Clearing\n");
			case RESIZE:
				printf("Resizing\n");
			case PRINT_BUCKET:
				printf("Print Bucket\n");
			default:
				printf("Default code\n");
			}
			fclose(file);
		}
	}
	return 0;
}
