#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern float computeArea(int *v1, int *v2, int shape);
extern int numTimesAppears(char *mystring, char ch);


int main(void) {
  int i;
  char mystring[100] = "Professor Ken";
  char ch;
  float area;
  int  length, height, diameter, base;

  int count;

 base = 25;
 length  = 25;
 height  = 10;
 diameter = 5;

  area = computeArea(&base, &height, 1);
  printf("\nThe area of the triangle is %f", area);

  area = computeArea(&height, &length, 2);
  printf("\nThe area of the rectangle is %f", area);


  area = computeArea(&diameter, &diameter,3);
  printf("\nThe area of a circle is %f", area);


  ch = 'o';
  count = numTimesAppears(mystring, ch);
  printf("\n Number of times %c appears in string is %d", ch, count);

  return 0;

}

float  computeArea (int *v1, int *v2, int shape) {
	float areaComputed;

	switch(shape) {

	case 1: // triangle
		areaComputed = 0.5 * *v1 * *v2; // 1/2 * base * height
		break;

	case 2: // rectangle
		areaComputed = *v1 * *v2; // height * width
		break;

	case 3: // circle
		areaComputed = (0.25 * *v1 * *v1) * M_PI; // (1/4) * diameter^2 * pi
		break;
	default:
		printf("NOT A VALID CASE: 1 - triangle, 2 - rectangle, 3 - circle\n");
		areaComputed = -1;
		break;
	}

   return(areaComputed);

}


int numTimesAppears(char  *mystring, char ch)
{

  int i = 0; // start at first char
  int count = 0;

  while(mystring[i] != '\0') { // loop until end of string
	  if(mystring[i] == ch) {
		  ++count; // increment count
	  }
	  ++i;
  }

  return(count);

}
