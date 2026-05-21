/*
 ============================================================================
 Name        : RomanNumerals.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 15

int main(void) {


	char input[SIZE];
	unsigned int value = 0, sum = 0;

	strcpy(input, "MDCLXIII");

	for (size_t i = 0; i < strlen(input); ++i) {
		switch(input[i]) {
			case 'I': // 1
				value = 1;
				break;

			case 'V': // 5
				value = 5;
				break;

			case 'X': // 10
				value = 10;
				break;

			case 'L': // 50
				value = 50;
				break;

			case 'C': // 100
				value = 100;
				break;

			case 'D': // 500
				value = 500;
				break;

			case 'M': // 1000
				value = 1000;
				break;
		}
		sum += value;
	}


	printf("%u", sum);

	return EXIT_SUCCESS;
}
