/*
** Assignment: Laboratory Assignment 01 "Hello World"
**
** Filename: helloWorld.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 8/29/2023, 9PM
**
** Description: This program prints the string "hello, world" with a newline character along with another string followed by
** a newline character. The intent
** of this program is to introduce beginning programmers to the C programming language.
**
** Input: None.
**
** Output: The string "hello, world" with a newline character. Another string "my name is Rhilo Novuno Sotto and my SIS ID number is 130551574"
**
** Algorithm: Output a string to stdout (the console) using the standard I/Oprintf() function.
**
** Caveats: Only able to create a single output.
** Version: 1.0
**
*/

#include <stdio.h>

int main(void) {

	int redId;

	redId = 130551574;

	printf("%s","hello, world\n");
	printf("my name is Rhilo Novuno Sotto and my SIS User ID number is %d\n", redId);


	return 0;
}
