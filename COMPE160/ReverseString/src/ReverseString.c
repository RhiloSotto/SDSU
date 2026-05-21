/*
** Assignment: Laboratory Assignment 10 "Recursive Reverse String"
**
** Filename: ReverseString.c
**
** Author: Rhilo Novuno Sotto
** REDID: 130551574
**
** Lab Section: 6719
** Lab TA: Jingxiao Tian
** Due Date: 10/31/2023, 9PM
**
** Description: given an input string, reverse it recursively
**
** Input: a string
**
** Output: the string reversed
**
** Algorithm: get string from stdin, call reversestring function, which is a recursive
** function to move the last character to the first position and add an end of string character
**
** Caveats: input string has a max length of 50
**
** Version: 1.0
**
*/

#include <stdio.h>
#include <string.h>

char* ReverseString(char* stringToReverse) {
   /* TODO: Complete recursive ReverseString() function here. */
	 if (strlen(stringToReverse) <= 1) {
	        return stringToReverse;
	    } else {
	        char temp = stringToReverse[0];
	        stringToReverse[0] = stringToReverse[strlen(stringToReverse) - 1];
	        stringToReverse[strlen(stringToReverse) - 1] = '\0';
	        ReverseString(stringToReverse + 1);
	        stringToReverse[strlen(stringToReverse)] = temp;
	        return stringToReverse;
	    }
	}

int main(void) {
   char inStr[50];
   char* resultStr;

   fgets(inStr, 20, stdin);
   strtok(inStr, "\n");  // Remove newline character from input.
   inStr[strlen(inStr) - 1] = '\0'; // Removes newline character from the input
   resultStr = ReverseString(inStr);
   printf("Reversed: %s\n", resultStr);

   return 0;
}
