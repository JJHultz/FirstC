#include <stdio.h>

int main() {
	char input[100]; // a buffer for the input - see below
	int count;

	printf("Enter a string: ");
	scanf("%99s", input); // read a word (no spaces)
	
	printf("How many times? ");
	scanf("%d", &count);

	for (int i = 0; i < count; i++) {
		printf("%s\n", input);
	}
	
	return 0;
}
