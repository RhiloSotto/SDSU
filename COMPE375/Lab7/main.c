/*
 * Lab7.c
 *
 * Created: 11/5/2024 1:30:00 PM
 * Author : rnsot
 */ 

#define F_CPU 16000000UL // 16MHz clock frequency

#include <avr/io.h>
#include <avr/interrupt.h>

// REDID: 130551574
// X = 5
//	Timer interrupt period = (5 + 1) ms = 6ms
// Y = 7
// Z = 4
//	PWM frequency = (4 + 1) * 100Hz = 500Hz
//		2 ms period

uint16_t dutyCycle = 0;

void init(void) {
	DDRB |= (1 << DDRB5); // output to LED
}

void adc_init(void) {
	DDRC &= ~(1 << 1); // Y = 7	% 6 = 1
	ADMUX |= (1 << REFS0) | (1 << MUX0); // VCC reference, ADC1 (PINC1)
	ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE); 
	// enable ADC, auto trigger, and conversion complete interrupt
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// Prescaler x128, 125kHz frequency (between 50kHz and 200kHz)
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0); // triggers on timer0 compare match A
}

void timer_init(void) {
	// FCPU = 16MHz, want 6ms and and 2ms
	// TIMER 0 - interrupt timer (6ms)
	// ps = 1024, NM -> 16384us = 16.384ms **
	
	TCCR0A |= (1 << WGM01); // CTC Mode
	OCR0A = 93; // timer period (6ms/16.384ms * 256 time steps = 93.75 time steps - 1 = 93 OCRA)
	
	TIMSK0 |= (1 << OCIE0A); // set ISR COMPA vector
	
	// TIMER 2 - PWM timer (2ms)
	// ps = 128, NM -> 2048us = 2.048ms **
	
	TCCR2A |= (1 << WGM21); // CTC Mode
	
	OCR2A = 249; // timer period (2ms/2.048ms * 256 time steps = 250 time steps - 1 = 249 OCRA)

	TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B); // set ISR COMPA and COMPB vector

	TCCR0B |= (1 << CS02) | (1 << CS00); // Pre-scaler set to 1024, STARTS timer0
	TCCR2B |= (1 << CS22) | (1 << CS20); // Pre-scaler set to 128, STARTS timer2
}


int main(void) {

	init(); // initialize inputs and outputs
	adc_init(); // initialize ADC settings
	timer_init(); // timer settings

	sei(); // enable global interrupts


    while (1) {
	}
		
}

ISR (TIMER0_COMPA_vect) { // Timer0 compare register A interrupt
	// start ADC conversion
	ADCSRA |= (1 << ADSC);
}

ISR (ADC_vect) { // ADC auto trigger and conversion complete interrupt
	uint16_t variable = ADC;
	dutyCycle = (OCR2A * (variable >> 2)) >> 10; // shift 10 bit ADC result and divide by 1024
	OCR2B = dutyCycle; // adjusts LED brightness

}

ISR (TIMER2_COMPA_vect) { // Timer2 compare register A interrupt
	if (OCR2B > 0) PORTB |= (1 << PORTB5); // HIGH, LED ON
}

ISR (TIMER2_COMPB_vect) { // Timer2 compare register B interrupt
	PORTB &= ~(1 << PORTB5); // LOW, LED OFF
}

