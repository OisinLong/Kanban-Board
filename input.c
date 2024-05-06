#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "input.h"
#include "output.h"

short int GetNumInRange(short int min, short int max) {
	short int num = -1;
	// Will iterate until a valid number is entered
	while (num < min || num > max) {
		scanf("%hu", &num);

		if (num < min || num > max) {
			printf(RED "\n#Error: Input Must be Between %hu and %hu Inclusive#\n" RESET, min, max);
			FlushInput();
		}
	}
	FlushInput();
	return(num);
}

// Cleans the input buffer to prevemt errors when reading from stdin
void FlushInput() {
	char character;
	do {
		character = getchar();

	} while (character != '\n' && character != EOF);
}