#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define NOT_OK 0

int main(int argc, char **argv){
	int number;
	if (argc < 2)
		return;
	char *p = argv[1];
	int state = OK;
	while (*p != '\0'){
		if((*p - '0') >= 0 && (*p - '0') <= 9) {
		printf("%c", *p);
		} else {
		printf("%c\n", *p);
		printf("Not OK\n");
		}
		++p;
	}
	return 0;
}
