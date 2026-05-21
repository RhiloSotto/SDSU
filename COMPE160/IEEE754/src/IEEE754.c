/*
** Assignment: Laboratory Assignment 14 "IEEE half-precision (FP16)"
**
** Filename: IEEE754.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: //2023, 9PM
**
** Description: Takes two octets string and outputs the value according to
** IEEE half-precision (FP16)
**
** Input: two octets
**
** Output: decimal value of FP16 string
**
** Algorithm: Converts two octet strings into decimal values placed into
** unsigned short variables, bit shifting the first string into the left-most
** bits and adding the two variables together to form a 16 bit long sequence.
** using masks on the acquired sequence to obtain the sign bit, exponential bits,
** and mantissa bits, and using the FP16 formula to obtain the decimal value
**
** Caveats: Can only deal with FP16 strings
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string.h>

#define CHAR_BIT 8



unsigned short getDecimal(char* string) { // converts binary string to decimal value
	unsigned short decimalRep = 0;
	unsigned short base = 1; // 2^0
	for (unsigned int c = 1; c <= (CHAR_BIT * sizeof(unsigned short) / 2); ++c) {
		decimalRep += (string[(CHAR_BIT * sizeof(unsigned short) / 2) - c] - '0') * base;
		base *= 2;
	}

	return decimalRep;
}


unsigned short getSign(unsigned short value) {
	// get bit 15
	unsigned short displayMask = 1 << (CHAR_BIT * sizeof(unsigned short) - 1);
	return (value & displayMask) >> 15;
}

unsigned short getExponent(unsigned short value) {
	// get bits 14-10
	unsigned short displayMask = 31 << 10; // 31(10) = 11111(2)
	return ((value & displayMask) >> 10);
}

unsigned short getMantissa(unsigned short value) {
	// get bits 9-0
	unsigned short displayMask = 1023; // 1023(10) = 1111111111(2)
	return (value & displayMask);
}


int main(int argc, char* argv[]) {

	/* manually testing console values
	char users[][8] = {
			{'0','0','0','0','0','1','0','0'},
			{'0','0','0','0','0','0','0','0'}
	};
	*/
	unsigned short a = 0; // getDecimal(users[0]);
	unsigned short b = 0; // getDecimal(users[1]);



	// the following checks if parameters are correct for program
	if (argc != 3) {
		puts("usage: FP16 <msb> <lsb>"); // usage message
		exit(0);
	}
	else {
		char *endp;
		a = strtod(argv[1],&endp);
		a = getDecimal(argv[1]);
		// printf("a = %u\n", a); checking a value
		if (argv[1] == endp || *endp != '\0') {
			puts("usage: FP16 <msb> <lsb>"); // usage message
			exit(0);
		}
		b = strtod(argv[2],&endp);
		b = getDecimal(argv[2]);
		// printf("b = %u\n", b); // checking b value
		if (argv[2] == endp || *endp != '\0') {
			puts("usage: FP16 <msb> <lsb>"); // usage message
			exit(0);
		}
	}

	unsigned short FP16 = 0;
	unsigned short sign = 0, exponent = 0, mantissa = 0;

	a = a << 8;
	FP16 = a + b;

	// printf("%u %u = %u\n", a, b, FP16); checking values

	sign = getSign(FP16);
	exponent = getExponent(FP16);
	mantissa = getMantissa(FP16);

	// printf("%u %u %u\n", sign, exponent, mantissa); // checking values

	printf("%s %s = ", argv[1], argv[2]);

	if (exponent == 0) { // all exponent bits are 0
		printf("%.20lf\n", pow(-1,sign) * pow(2,-14) * (mantissa / pow(2,10)));

	}
	else if (exponent == 31) { // all exponent bits are 1
		if (sign == 1)
			printf("-");
		printf("infinity\n");
	}
	else {
		printf("%.20lf", pow(-1,sign) * pow(2, exponent - 15) * (1 + (mantissa / pow(2,10))));

	}

	return 0;
}
