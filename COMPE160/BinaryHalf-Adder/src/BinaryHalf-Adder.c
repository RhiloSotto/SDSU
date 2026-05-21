/*
** Assignment: Laboratory Assignment 03 "Binary Half-Adder"
**
** Filename: BinaryHalf-Adder.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 9/12/2023, 9PM
**
** Description: Replicates a circuit that adds two bits called a binary half-adder
** based on a logical schematic
**
** Input: 2 real integers, intended to be '0' and '1'
**
** Output: Carry is either 0 or 1, Sum is either 0 or 1
**
** Algorithm: runs two if statements following user input, performs logical operations
** to determine bitwise sum and carry
**
** Caveats: user can use any integer for the inputs, not necessarily '0' or '1'
** program regardless treats non-zero integers as binary '1'
**
** Version: 1.0
**
*/

#include <stdio.h>

int main(void) {

	int a, b; // user inputs
	int sum = 0, carry = 0; // initialized to 0

	printf("Input binary symbol for A:\n");
	fflush(stdout); // workaround for scanf not working properly in Eclipse IDE
	scanf("%d", &a);

	printf("Input binary symbol for B:\n");
	fflush(stdout); // same use case as above
	scanf("%d", &b);


	if (a || b) { // sum logic gates
		sum = (a||b)&&(!(a&&b)); // Sum = (A OR B) AND NOT(A AND B)
	}

	if (a && b) { // carry logic gates
		carry = (a&&b); // Carry = A AND B
	}

	printf("Carry is %d, Sum is %d\n", carry, sum); // result displayed

	return 0;
}
