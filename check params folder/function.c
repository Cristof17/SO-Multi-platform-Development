#include <stdio.h>

int check_if_number(char *string){
	char *p = string;
	while (*p != '\0'){
	int decimal_val = *p - '0';
		if (!(decimal_val > 0 && decimal_val < 9)){
			return 0;
		}
	++p;
	}
	return 1;
}

int main(int argc, char **argv) {
	if (argc < 2)
		return 1;
	printf("%d\n", check_if_number(argv[1]));
	
	return 0;
}
