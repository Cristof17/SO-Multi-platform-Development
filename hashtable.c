#include <stdio.h>
#include "hash.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

#define NO_ERROR 10
#define ERROR_FOUND 11

char errorString[512];

struct node {
	char *cuvant;
	struct node *next;
	struct node *prev;
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
	struct node *iterator;
	struct node *new_node;
	int is_null = 0;

	if (hashtable == NULL) {
		/*
		 * HashTable is null error
		 */
	}
	hash_code = hash(word, hashtable->size);
	if (hashtable->buckets == NULL) {

		uint32_t lungime = hashtable->size;
		int i;

		hashtable->buckets = (struct bucket **)malloc(hashtable->size *
						sizeof(struct bucket *));
		for (i = 0; i < lungime; ++i)
			hashtable->buckets[i] = NULL;
	}
	if (hashtable->buckets[hash_code] == NULL) {
		hashtable->buckets[hash_code] = (struct bucket *)
					malloc(1 * sizeof(struct bucket));
		hashtable->buckets[hash_code]->top = NULL;
	}

	iterator = hashtable->buckets[hash_code]->top;
	if (iterator == NULL)
		is_null = 1;

	new_node = malloc(1 * sizeof(struct node));
	new_node->cuvant = malloc(BUFFSIZE * sizeof(char));
	new_node->next = NULL;
	new_node->prev = NULL;
	strcpy(new_node->cuvant, word);

	if (iterator != NULL && is_null == 0) {
		/*
		 *Check duplicates
		 */
		while (iterator != NULL && iterator->cuvant != NULL) {
			if (strcmp(iterator->cuvant, word) == 0) {
				free(new_node);
				return;
			}
			if (iterator->next == NULL) {
				iterator->next = new_node;
				new_node->prev = iterator;
				return;
			}
			iterator = iterator->next;
		}
	} else {
		hashtable->buckets[hash_code]->top = new_node;
	}
}

void remove_element(struct hashtable *hashtable, char *object)
{

	int hash_code = hash(object, hashtable->size);
	struct bucket *target = hashtable->buckets[hash_code];
	struct node *top;
	struct node *tmp;

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
				/*
				 *If is last word in list
				 */
				tmp = top->next->next;
				free(top->next);
				top->next = NULL;
				top->next = tmp;
				if (top->next != NULL)
					top->next->prev = top;
				return;
			}
			top = top->next;
		}
	}
}

int find(struct hashtable *hashtable, char *word, char *filename)
{
	int hash_code;
	struct node *it;
	struct bucket *bucket;

	hash_code = hash(word, hashtable->size);
	bucket = hashtable->buckets[hash_code];
	if (bucket == NULL)
		return 0;
	it = bucket->top;
	while (it != NULL) {
		if (strcmp(it->cuvant, word) == 0)
			return 1;
		it = it->next;
	}
	return 0;
}

void print(struct hashtable *hashtable, char *filename)
{
	if (filename == NULL) {

		struct node *it;
		int i;

		for (i = 0; i < hashtable->size; ++i) {
			/*
			 * The bucket is empty
			 */
			if (hashtable->buckets[i] == NULL)
				continue;
			it = hashtable->buckets[i]->top;
			if (it == NULL || it->cuvant == NULL)
				continue;
			printf("%s ", it->cuvant);
			it = it->next;
			while (it != NULL) {
				printf("%s ", it->cuvant);
				it = it->next;
			}
			printf("\n");
		}

	} else {

		FILE *file;
		int i;
		struct node *it;

		file = fopen(filename, "a");
		if (file == NULL) {
			perror(errorString);
			printf("%s\n", errorString);
			return;
		}
		/*
		 * Print contents of every bucket
		 */
		for (i = 0; i < hashtable->size; ++i) {
			/*
			 * The bucket is empty
			 */
			if (hashtable->buckets[i] == NULL)
				continue;
			it = hashtable->buckets[i]->top;
			if (it == NULL)
				continue;
			fprintf(file, "%s ", it->cuvant);
			while (it->next != NULL) {
				it = it->next;
				fprintf(file, "%s ", it->cuvant);
			}
			fprintf(file, "\n");
		}
		fflush(file);
		fclose(file);
	}
}

void print_bucket(struct hashtable *hashtable, char *index, char *filename)
{
	int hash_code = atoi(index);
	//printf("Atoi(index) = %d\n", atoi(index));
	//TODO Check if the index is valid
	char *k = index;
	int j = 0;
	/*
	for (j = 0; j < strlen(index); ++j) {
		if (!(*k - '0' >= 0 && *k - '0' < 9))
			//printf("%c not valid\n", *k);
			return;
		++k;
	}
	*/
	struct bucket *bkt = hashtable->buckets[hash_code];
	struct node *it;
	FILE *file;
	int i = 0;

	if (hash_code >= hashtable->size)
		return;

	if (filename != NULL) {
		file = fopen(filename, "wa+");
		if (file == NULL) {
			perror(errorString);
			printf("%s\n", errorString);
		}
	}
	if (bkt == NULL)
		return;

	it = bkt->top;
	do {
		if (filename == NULL)
			printf("%s ", it->cuvant);
		else
			fprintf(file, "%s ", it->cuvant);
		it = it->next;
		++i;
	} while (it != NULL);
	if (filename == NULL)
		printf("\n");
	else
		fprintf(file, "\n");
}

void clear_nodes(struct hashtable *hashtable)
{

	struct bucket *bucket;
	struct node *it;
	int i;

	for (i = 0; i < hashtable->size; ++i) {
		bucket = hashtable->buckets[i];
		if (bucket == NULL)
			continue;
		it = bucket->top;
		/*
		 *Get to the end of the bucket and release every resource
		 */
		if (it == NULL || it->cuvant == NULL)
			continue;
		while (it->next != NULL)
			it = it->next;
		/*
		 *From the end to the begining
		 */

		while (it != NULL) {
			if (it->cuvant != NULL) {
				free(it->cuvant);
				it->cuvant = NULL;
			}
			if (it->next != NULL) {
				free(it->next);
				it->next = NULL;
			}

			if (it->prev == NULL) {
				free(it);
				it = NULL;
				hashtable->buckets[i]->top = NULL;
				break;
			}
			it = it->prev;
			if (it != NULL) {
				if (it->next != NULL) {
					free(it->next);
					it->next = NULL;
				}
			}
		}
	}
}

void clear_buckets(struct hashtable *hashtable)
{
	int i = 0;
	struct bucket *bkt;

	for (i = 0; i < hashtable->size; ++i) {
		bkt = hashtable->buckets[i];
		if (bkt == NULL)
			continue;
		if (bkt->top != NULL) {
			if (bkt->top->cuvant == NULL)
				continue;
			if (bkt->top->next != NULL) {
				free(bkt->top->next);
				bkt->top->next = NULL;
			}
			if (bkt->top->prev != NULL) {
				free(bkt->top->prev);
				bkt->top->prev = NULL;
			}
			if (bkt->top->cuvant != NULL)
				bkt->top->cuvant = NULL;
			bkt->top = NULL;
		}
	}
}

void clear(struct hashtable *hashtable)
{
	clear_nodes(hashtable);
	clear_buckets(hashtable);
}

struct hashtable *resize_halve(struct hashtable *hashtable
				, struct hashtable *new)
{
	uint32_t size = hashtable->size;
	struct bucket *bkt;
	struct node *it;
	int i;

	new = malloc(1 * sizeof(struct hashtable));
	new->size = size/2;
	new->buckets = NULL;
	for (i = 0; i < size; ++i) {
		bkt = hashtable->buckets[i];
		if (bkt == NULL)
			continue;
		it = bkt->top;
		while (it != NULL) {
			if (it->cuvant == NULL)
				break;
			add(new, it->cuvant);
			it = it->next;
		}
	}
	clear_nodes(hashtable);
	clear_buckets(hashtable);
	return new;
}

struct hashtable *resize_double(struct hashtable *hashtable
				, struct hashtable *new)
{
	uint32_t size = hashtable->size;
	struct bucket *bkt;
	struct node *it;
	int i;

	new = malloc(1 * sizeof(struct hashtable));
	new->size = 2 * size;
	new->buckets = NULL;
	for (i = 0; i < size; ++i) {
		bkt = hashtable->buckets[i];
		if (bkt == NULL)
			continue;
		it = bkt->top;
		while (it != NULL) {
			if (it->cuvant == NULL)
				break;
			add(new, it->cuvant);
			it = it->next;
		}
	}
	clear_nodes(hashtable);
	clear_buckets(hashtable);
	return new;
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

struct hashtable *process_input(struct hashtable *hashtable
				, char *buffer, uint32_t lungime, int *error)
{
	char *token;
	int opcode;
	*error = NO_ERROR;

	token = (char *)strtok(buffer, "\n ");
	if (token == NULL)
		return hashtable;
	opcode = get_operation_code(token);
	switch (opcode) {
	case ADD:
	{
		char *argument;
		argument = (char *)strtok(NULL, "\n ");
		if (argument == NULL) {
			*error = ERROR_FOUND;
			break;
		}
		add(hashtable, argument);
		break;
	}
	case PRINT:
	{
		char *outputFile;

		outputFile = (char *)
				strtok(NULL, "\n ");
		print(hashtable, outputFile);
		break;
	}
	case FIND:
	{
		int found;
		char *out;
		char *word;

		word = (char *)strtok(NULL, "\n ");
		out = (char *)strtok(NULL, "\n ");
		found = find(hashtable, word, out);
		if (out == NULL) {
			if (found)
				printf("True\n");
			else
				printf("False\n");
		} else {
			FILE *output = fopen(out, "a");

			if (output == NULL)
				perror(errorString);
			if (found)
				fprintf(output, "True\n");
			else
				fprintf(output, "False\n");
			fflush(output);
			fclose(output);
		}
		break;
	}
	case REMOVE:
	{
		char *argument = (char *)
				strtok(NULL, "\n ");
		remove_element(hashtable, argument);
		break;
	}
	case CLEAR:
	{
		clear(hashtable);
		break;
	}
	case RESIZE:
	{
		char *dimen = (char *)strtok(NULL, "\n ");
		struct hashtable *new = NULL;

		if (strcmp(dimen, "halve") == 0)
			hashtable = resize_halve(hashtable, new);
		else if (strcmp(dimen, "double") == 0)
			hashtable = resize_double(hashtable, new);
		break;
	}
	case PRINT_BUCKET:
	{
		//TODO Check for error
		char *index = (char *)strtok(NULL, "\n ");
		char *out = (char *)strtok(NULL, "\n ");

		print_bucket(hashtable, index, out);
		break;
	}
	default:
		//printf("No command found\n");
		*error = ERROR_FOUND;
		break;
	}
	return hashtable;
}

int main(int argc, char **argv)
{
	int i;
	uint32_t lungime;
	struct hashtable *hashtable;
	char *buffer;
	int error;

	/* reading argc to know if there is any input files */
	if (argc == 1)
		//TODO Put DIE
		return -1;
	lungime = (uint32_t)atoi(argv[1]);
	if (lungime < 0)
		fprintf(stderr, "Lungimea hashtable-ului nu e +");

		hashtable = (struct hashtable *)
					malloc(1 * sizeof(struct hashtable));
		hashtable->size = lungime;
		hashtable->buckets = NULL;
	if (argc == 2) {
		buffer = (char *)malloc(BUFFSIZE * sizeof(char));
		while (fgets(buffer, BUFFSIZE, stdin)) {
			hashtable = process_input(hashtable, buffer, lungime,
								 &error);
			if (error == ERROR_FOUND)
				return -1;
		}
	}
	for (i = 2; i < argc; ++i) {
		FILE *file = fopen(argv[i], "r+");

		if (file == NULL) {
			perror(errorString);
			return -1;
		} else if (errno == ENOENT) { //No such file or directory
			//If the struct is not null but there is no file
			perror(errorString);
			return -1;
		}

		/*
		 * Read from file line by line
		 */
		buffer = (char *)malloc(BUFFSIZE * sizeof(char));
		while (fgets(buffer, BUFFSIZE, file)) {
			hashtable = process_input(hashtable, buffer, lungime,
								 &error);
			if (error == ERROR_FOUND)
				return -1;
		}
		fclose(file);
	}
	return 0;
}
