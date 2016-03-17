#include <stdio.h>
#include "hash.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
	uint32_t size;
	struct bucket **buckets;
};

void add(struct hashtable *hashtable, char *word)
{
	unsigned int hash_code;
	struct node *new_node;

	if (hashtable == NULL) {
		/*
		 * TODO
		 * HashTable is null error
		 */
	}
	hash_code = hash(word, hashtable->size);
	if (hashtable->buckets == NULL) {
		hashtable->buckets = (struct bucket **)malloc(hashtable->size *
						sizeof(struct bucket *));
	}
	new_node = (struct node *)malloc(1 * sizeof(struct node));
	new_node->cuvant = malloc(BUFFSIZE * sizeof(char));
	strcpy(new_node->cuvant, word);
	if (hashtable->buckets[hash_code] == NULL) {
		hashtable->buckets[hash_code] = (struct bucket *)
					malloc(1 * sizeof(struct bucket));
	}
	struct node *iterator = hashtable->buckets[hash_code]->top;
	if (iterator != NULL) {
		/*
		 *Check duplicates
		 */
		while (iterator != NULL) {
			if (strcmp(iterator->cuvant, word) == 0) {
				printf("Not adding duplicate\n");
				free(new_node);
				return;
			}
			iterator = iterator->next;
			if (iterator->next == NULL) {
				iterator->next = new_node;
			}
		}
	} else {
		hashtable->buckets[hash_code]->top = new_node;
	}
}

void remove_element(struct hashtable *hashtable, char *object)
{

	int hashcode = hash(object, hashtable->size);
	struct bucket *target = hashtable->buckets[hashcode];
	struct node *top;

	if (target == NULL)
		return;

	top = target->top;
	/*
	 * Check current node
	 */
	if (strcmp(top->cuvant, object) == 0) {
		target->top = top->next;
		free(top);
	} else {
		while (top->next != NULL) {

			char *next_word = top->next->cuvant;

			if (strcmp(next_word, object) == 0) {
				top->next = top->next->next;
				free(top->next);
			}
			top = top->next;
		}
	}
}

void print(struct hashtable *hashtable, char *filename)
{
	if (filename == NULL) {

		struct node *it;
		int i;

		printf("Filename is NULL\n");
		for (i = 0; i < hashtable->size; ++i) {
			/*
			 * The bucket is empty
			 */
			if (hashtable->buckets[i] == NULL) {
				continue;
			}
			printf("i = %d AAAAAAA\n", i);
			it = hashtable->buckets[i]->top;
			printf("%s ", it->cuvant);
			while (it->next != NULL) {
				it = it->next;
				printf("%s ", it->cuvant);
			}
			printf("\n");
		}

	} else {

		FILE *file;
		int i;
		struct node *it;

		printf("Filename is not NULL\n");
		file = fopen(filename, "wa");
		/*
		 * Print contents of every bucket
		 */
		for (i = 0; i < hashtable->size; ++i) {
			/*
			 * The bucket is empty
			 */
			if (hashtable->buckets[i] == NULL) {
				continue;
			}
			it = hashtable->buckets[i]->top;
			fprintf(file, "%s ", it->cuvant);
			while (it->next != NULL) {
				it = it->next;
				fprintf(file ,"%s ", it->cuvant);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}
}

int get_operation_code(char *operation)
{
	if (strcmp(operation, "add") == 0)
		return ADD;
	if (strcmp(operation, "remove") == 0)
		return REMOVE;
	if (strcmp(operation, "find") == 0)
		return FIND;
	if (strcmp(operation, "clear") == 0)
		return CLEAR;
	if (strcmp(operation, "print_bucket") == 0)
		return PRINT_BUCKET;
	if (strcmp(operation, "print") == 0)
		return PRINT;
	if (strcmp(operation, "resize") == 0)
		return RESIZE;
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
	lungime = (uint32_t)atoi(argv[1]);
		hashtable = (struct hashtable *)
					malloc(1 * sizeof(struct hashtable));
		hashtable->size = lungime;
	if (argc == 2) {
		/* TODO
		 * READ FROM STDIN
		 */
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
			while (fgets(buffer, BUFFSIZE, file)) {
				token = (char *)strtok(buffer, "\n ");
				printf("token = %s\n", token);
				opcode = get_operation_code(token);
				printf("opcode = %d\n", opcode);
				switch (opcode) {
				case ADD:
				{
					char *argument;

					argument = (char *)strtok(NULL, " ");
					printf("Argument is %s\n", argument);
					add(hashtable, argument);
					break;
				}
				case PRINT:
				{
					char *outputFile;

					outputFile = (char *)strtok(NULL, " ");
					print(hashtable, outputFile);
					printf("Printing hashtable\n");
					break;
				}
				case FIND:
					printf("Finding word\n");
					break;
				case REMOVE:
				{
					char *argument = (char *)
							strtok(NULL, "\n ");
					remove_element(hashtable, argument);
					printf("Removing word\n");
					break;
				}
				case CLEAR:
					printf("Clearing\n");
					break;
				case RESIZE:
					printf("Resizing\n");
					break;
				case PRINT_BUCKET:
					printf("Print Bucket\n");
					break;
				default:
					printf("Default code\n");
					break;
				}
			}
			fclose(file);
		}
	}
	return 0;
}
