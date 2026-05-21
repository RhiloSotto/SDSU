/*
 * Lab1.c
 *
 * Created: 9/5/2024 2:25:19 PM
 * Author : rnsot
 */ 

// 1 unit is 200ms
#define F_CPU 16000000UL  // 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>

void dot(void) {
	PORTB |= (1<<PORTB5); // light on
	_delay_ms(200); // 1 unit
	PORTB &= ~(1<<PORTB5); // light off
	_delay_ms(200); // 1 unit

}

void dash(void) {
	PORTB |= (1<<PORTB5); // light on
	_delay_ms(600); // 3 units
	PORTB &= ~(1<<PORTB5); // light off
	_delay_ms(200); // 1 unit

}

void letterSpace(void) {
	PORTB &= ~(1<<PORTB5); // ensure light is off
	_delay_ms(400); // 3 units (2 units + 1 unit is implicit with dot/dash function)
}

void wordSpace(void) {
	PORTB &= ~(1<<PORTB5); // ensure light is off
	_delay_ms(800); // 7 units (4 units + 2 units is implicit since letterSpace() is always called + 1 unit is implicit with dot/dash function)
}

void morseCode(char* string) {
	uint8_t stringIndex = 0;
	while(string[stringIndex] != '\0') { // until the end of string is reached
		switch (string[stringIndex]) {
			// letters
			case 'A':
			case 'a':
				dot(); dash();
				break;
			case 'B':
			case 'b':
				dash(); dot(); dot(); dot();
				break;
			case 'C':
			case 'c':
				dash(); dot(); dash(); dot();
				break;
			case 'D':
			case 'd':
				dash(); dot(); dot();
				break;
			case 'E':
			case 'e':
				dot();
				break;
			case 'F':
			case 'f':
				dot(); dot(); dash(); dot();
				break;
			case 'G':
			case 'g':
				dash(); dash(); dot();
				break;
			case 'H':
			case 'h':
				dot(); dot(); dot(); dot();
				break;
			case 'I':
			case 'i':
				dot(); dot();
				break;
			case 'J':
			case 'j':
				dot(); dash(); dash(); dash();
				break;
			case 'K':
			case 'k':
				dash(); dot(); dash();
				break;
			case 'L':
			case 'l':
				dot(); dash(); dot(); dot();
				break;
			case 'M':
			case 'm':
				dash(); dash();
				break;
			case 'N':
			case 'n':
				dash(); dot();
				break;
			case 'O':
			case 'o':
				dash(); dash(); dash();
				break;
			case 'P':
			case 'p':
				dot(); dash(); dash(); dot();
				break;
			case 'Q':
			case 'q':
				dash(); dash(); dot(); dash();
				break;
			case 'R':
			case 'r':
				dot(); dash(); dot();
				break;
			case 'S':
			case 's':
				dot(); dot(); dot();
				break;
			case 'T':
			case 't':
				dash();
				break;
			case 'U':
			case 'u':
				dot(); dot(); dash();
				break;
			case 'V':
			case 'v':
				dot(); dot(); dot(); dash();
				break;
			case 'W':
			case 'w':
				dot(); dash(); dash();
				break;
			case 'X':
			case 'x':
				dash(); dot(); dot(); dash();
				break;
			case 'Y':
			case 'y':
				dash(); dot(); dash(); dash();;
				break;
			case 'Z':
			case 'z':
				dash(); dash(); dot(); dot();
				break;
			// numbers
			case '1':
				dot(); dash(); dash(); dash(); dash();
				break;
			case '2':
				dot(); dot(); dash(); dash(); dash();
				break;
			case '3':
				dot(); dot(); dot(); dash(); dash();
				break;
			case '4':
				dot(); dot(); dot(); dot(); dash();
				break;
			case '5':
				dot(); dot(); dot(); dot(); dot();
				break;
			case '6':
				dash(); dot(); dot(); dot(); dot();
				break;
			case '7':
				dash(); dash(); dot(); dot(); dot();
				break;
			case '8':
				dash(); dash(); dash(); dot(); dot();
				break;
			case '9':
				dash(); dash(); dash(); dash(); dot();
				break;
			case '0':
				dash(); dash(); dash(); dash(); dash();
				break;
			// space between words and beginning of numbers
			case ' ':
				wordSpace();
				break;
			// fail case
			default:
				break;
		}
		letterSpace(); // always going to call letter space
		// delay of dot/dash = space between part of same letter
		// delay of dot/dash + letterSpace = space between letters
		// delay of dot/dash + wordSpace + letterSpace = space between words
		++stringIndex; // next character
	}
}

int main(void)
{	
	DDRB |= (1<<DDB5);  //0x20 (hex) // Set port bit B5 in data direction register to 1: an OUTput
	// TESTING ALL MORSE CODE
	// "A a B b C c D d E e F f G g H h I i J j K k L l M m N n O o P p Q q R r S s T t U u V v W w X x Y y Z z 1 2 3 4 5 6 7 8 9 0";
	
	// null terminated ASCII string
	// breaking up words and number with ASCII space character
	while(1) {
		morseCode("Rhilo Sotto 130551574");
		wordSpace(); // end of the loop means a new word
		letterSpace(); // gap between words is dot/dash(1) + word(4) + letter(2) = (7)
	}
	// "Rhilo Sotto 130551574"
	/*
	R - dot dash dot
	H - dot dot dot dot
	I - dot dot
	L - dot dash dot dot
	O - dash dash dash 
	
	S - dot dot dot
	O - dash dash dash
	T - dash
	T - dash
	O - dash dash dash
	
	1 - dot dash dash dash dash
	3 - dot dot dot dash dash
	0 - dash dash dash dash dash
	5 - dot dot dot dot dot
	5 - dot dot dot dot dot
	1 - dot dash dash dash dash
	5 - dot dot dot dot dot
	7 - dash dash dot dot dot
	4 - dot dot dot dot dash
	*/
}
