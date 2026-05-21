/*
** Assignment: Laboratory Assignment 02 "Current Calculator"
**
** Filename: CurrentCalculator.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 9/5/2023, 9PM
**
** Description: Calculates the current in amperes of a circuit with three resistors in parallel, given three resistance values and voltage.
**
** Input: four real values
**
** Output: the strings "Enter three resistances in ohms: ", "Enter the supply voltage in volts: ", and "The current is # amperes"
**
** Algorithm: prompts and scans three resistance values in ohms from user, saving inputs into double variables, asks for voltage in volts,
** calculating the harmonic average of the resistances and then the current which is displayed in amperes
**
** Caveats: required fflush function to correctly prompt user for inputs, issue with Eclipse IDE
** Version: 1.0
**
*/

#include <stdio.h>

int main(void) {

	double r1 = 0.0, r2 = 0.0, r3 = 0.0; // declaration
	double v = 0.0; // supply voltage DC
	double totalCircuitResistance = 0.0;
	double current = 0.0;

//	printf("hello"); // testing printf before scanf

	printf("%s","Enter three resistances in ohms: \n");
	fflush(stdout); // scanf taking precedence over printf, workaround for Eclipse IDE
	scanf("%lf", &r1);
	scanf("%lf", &r2);
	scanf("%lf", &r3);

//	printf("%lf, %lf, %lf?\n", r1, r2, r3); // checking inputs

	printf("%s", "Enter the supply voltage in volts: \n");
	fflush(stdout); // same use case as above
	scanf("%lf", &v);

	totalCircuitResistance = 1/ ((1/r1) + (1/r2) + (1/r3)); // harmonic average
	current = v/totalCircuitResistance;

	printf("The current is %0.2lf amperes\n", current);

	return 0;
}
