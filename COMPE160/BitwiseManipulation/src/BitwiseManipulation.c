/*
** Assignment: Laboratory Assignment 13 "Bitwise Manipulation"
**
** Filename: BitwiseManipulation.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 11/21/2023, 9PM
**
** Description: Given two 32-bit binary strings, outputs the bitwise AND, OR, and
** XOR as well as the number of ones in each output
**
** Input: two 32-bit binary strings
**
** Output: AND, OR, XOR of those two binary strings as well as number of ones
**
** Algorithm: checks if user input is valid, if so, gives AND, OR, and XOR
**
** Caveats: Limited to 32 bits for input and output
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>

#define CHAR_BIT 8

unsigned int getDecimal(char* string) { // converts binary string to decimal value
	unsigned int decimalRep = 0;
	unsigned int base = 1; // 2^0
	for (unsigned int c = 1; c <= (CHAR_BIT * sizeof(unsigned int)); ++c) {
		decimalRep += (string[(CHAR_BIT * sizeof(unsigned int)) - c] - '0') * base;
		base *= 2;
	}

	return decimalRep;
}


void displayBits(unsigned int value) {
	// 10000000 00000000 00000000 00000000 // mask
	// 00000000 00000000 00000000 00001010 // value = 10

	// declare displayMask and left shift 31 bits
	unsigned int displayMask = 1 << ((CHAR_BIT * sizeof(unsigned int)) - 1);
	// printf("%7u = ", value);

	unsigned int one = 0;
	// loop through bits
	for (unsigned int c = 1; c <= (CHAR_BIT * sizeof(unsigned int)); ++c) {
		putchar(value & displayMask ? '1': '0');


		if ((value & displayMask))
			++one;

		value <<= 1; // shift value left by 1;

		if (c % CHAR_BIT == 0) { // output a space after 8 bits
			putchar(' ');
		}
	}
	printf("(%u)", one);
	putchar('\n');
}


int main(int argc, char* argv[]) {

	unsigned int a = 0;
	unsigned int b = 0;

	// the following checks if parameters are correct for program
	if (argc != 3) {
		printf("usage: bits a b\n");
		exit(0);
	}
	else {
		char *endp;
		a = strtod(argv[1], &endp);
		a = getDecimal(argv[1]);
		// printf("a = %u\n", a); checking a value
		if (argv[1] == endp || *endp != '\0') {
			printf("error: a parameter incorrect\n");
			exit(0);
		}
		b = strtod(argv[2], &endp);
		b = getDecimal(argv[2]);
		// printf("b = %u\n", b); // checking b value
		if (argv[2] == endp || *endp != '\0') {
			printf("error: b parameter incorrect\n");
			exit(0);
		}
	}

	unsigned int k = 0;
	k = a & b; // bitwise AND
	printf("AND: ");
	displayBits(k);

	printf("OR : ");
	k = a | b; // bitwise OR
	displayBits(k);

	printf("XOR: ");
	k = a ^ b; // bitwise XOR
	displayBits(k);


	return 0;
}
