/*
 * Lab8.c
 *
 * Created: 11/15/2024 2:40:05 PM
 * Author : rnsot
 */ 

#define F_CPU 16000000UL // 16MHz clock frequency

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

uint8_t dutyCycle[] = {0, 8, 15, 23, 31, 39, 46, 54, 62, 69, 77};
uint8_t EEMEM address = 0xFF; // default value at EEPROM address

void init(void) {
	DDRB |= (1 << DDRB5); // output to LED
}

void timer_init(void) {
	// FCPU = 16MHz, want 5ms
	// TIMER 0 - interrupt timer (5ms)
	// ps = 1024, NM -> 16384us = 16.384ms **
	TCCR0A |= (1 << WGM01); // CTC Mode
	OCR0A = 77; // timer period (5ms/16.384ms * 256 time steps = 78.125 time steps - 1 = 77 OCRA)	
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B); // set ISR COMPA and COMPB vector
	TCCR0B |= (1 << CS02) | (1 << CS00); // Pre-scaler set to 1024, STARTS timer0
}

int main(void) {
	init();
	timer_init();
		
	uint8_t dutyCycleIndex = eeprom_read_byte(&address);
	if (dutyCycleIndex == 0xFF) { // default EEPROM value is 0xFF
		dutyCycleIndex = 0;
		eeprom_update_byte(&address, 0);
	}
	else {
		dutyCycleIndex = (dutyCycleIndex + 1) % 11;
		eeprom_update_byte(&address, dutyCycleIndex);
	}
	OCR0B = dutyCycle[dutyCycleIndex];
	
	sei();
	while (1) {}
}

ISR (TIMER0_COMPA_vect) { // Timer0 compare register A interrupt
	if (OCR0B != 0) PORTB |= (1 << PORTB5); // HIGH, LED ON
}

ISR (TIMER0_COMPB_vect) { // Timer0 compare register B interrupt
	if (OCR0B != OCR0A) PORTB &= ~(1 << PORTB5); // LOW, LED OFF
}
