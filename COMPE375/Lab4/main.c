/*
 * Lab4.c
 *
 * Created: 9/27/2024 1:06:04 PM
 * Author : rnsot
 */ 


#define F_CPU 16000000UL // 16MHz clock speed

#include <avr/io.h>


void init(void) { // initialize i/o
	DDRB |= (1 << DDRB5); // output to LED
		
	DDRB &= ~(1 << DDRB7); // set on board button to input
	PORTB |= (1 << PORTB7); // pull-up resistor for on-board button, default value (OFF) is 1
}

void timer_init(void) { // initialize timer settings
	// FCPU = 16MHz, want 10ms
	// ps = 1, NM -> 16us
	// ps = 1024, NM -> 16384us = 16.384ms
	
	// Set to CTC Mode
	TCCR0A |= (1 << WGM01);
	 
	// 256 steps * (10000us/16384us) = 156.25 steps - 1 = 155.25 (round down) = 155
	// ~10ms timer, 0x9B == 155
	OCR0A = 0x9B; 
	
	// start at 0% duty cycle
	OCR0B = 0x00;
	
	// Pre-scaler set to 1024, STARTS the timer
	TCCR0B |= (1 << CS02) | (1 << CS00);

}

void generate_PWM(void) { // generates a PWM waveform with variable duty cycle
	PORTB |= (1 << PORTB5); // LED on, HIGH
	while ( (TIFR0 & (1 << OCF0B)) == 0 ) {} // busy while until OCF0B flag == 1 in TIFR0 register
	TIFR0 |= (1 << OCF0B); // reset OCR0B overflow 
	
	PORTB &= ~(1 << PORTB5); // LED off, LOW
	while ( (TIFR0 & (1 << OCF0A)) == 0 ) {} // busy while until OCF0A flag == 1 in TIFR0 register
	TIFR0 |= (1 << OCF0A); // reset OCR0A overflow flag
}

int main(void) {
	init();	// set inputs and outputs
	timer_init(); // timer settings
	
    while (1) {
		if ( !(PINB & (1 << PINB7)) ) { // button is pressed
			if (OCR0B < OCR0A) {
				generate_PWM();
				++OCR0B; // increase duty cycle
			}
			else PORTB |= (1 << PORTB5); // don't turn LED off if OCR0B is OCR0A
		}
		else { // button is not pressed/released	
			if (OCR0B > 0) {
				generate_PWM();
				--OCR0B; // decrease duty cycle
			}
			else continue; // don't turn LED on at all if OCR0B is 0
		}
	}
	
}

