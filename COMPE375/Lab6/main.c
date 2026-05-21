/*
 * Lab6.c
 *
 * Created: 10/29/2024 1:34:31 PM
 * Author : rnsot
 */ 

#define F_CPU 16000000UL // 16MHz clock speed

#include <avr/io.h>
#include <avr/interrupt.h>

// REDID: 130551574
// X = 5
//	Timer interrupt period = (5 + 1) ms = 6ms
// Y = 7
// Z = 4
//	PWM frequency = (4 + 1) * 100Hz = 500Hz
//		2 ms period


// Global Variable
volatile short int button = -1;

short int buttonmap[4][4] =
{
	{1, 2, 3, 0},
	{4, 5, 6, 0},
	{7, 8, 9, 0},
	{0, 0, 0, 0}
};

void init(void) {
	DDRB |= (1 << DDRB5); // output to LED
}

void GPIO_init(void) {
	// Rows = PORTD, Columns = PORTB
	// set rows to output
	DDRD |= (1<<DDD4 | 1<<DDD5 | 1<<DDD6 | 1<<DDD7); // Port D (4-7)
	// set column to input
	DDRB &= ~(1<<DDB0 | 1<<DDB1 | 1<<DDB2 | 1<<DDB3); // Port B (0-3)
	// pull up property enabled (columns)
	PORTB |= (1<<PORTB0 | 1<<PORTB1 | 1<<PORTB2 | 1<<PORTB3);
	// rows to logic high
	PORTD |= (1<<PORTD4 | 1<<PORTD5 | 1<<PORTD6 | 1<<PORTD7);
}

void timer_init(void) {
	// FCPU = 16MHz, want 6ms and and 2ms
	// TIMER 0 - interrupt timer (6ms)
	// ps = 1, NM -> 16us = 0.016ms
	// ps = 8, NM -> 128us = 0.128ms
	// ps = 64, NM -> 1024us = 1.024ms
	// ps = 256, NM -> 4096us = 4.096ms
	// ps = 1024, NM -> 16384us = 16.384ms **	
	
	TCCR0A |= (1 << WGM01); // CTC Mode
		
	OCR0A = 93; // timer period (6ms/16.384ms * 256 time steps = 93.75 time steps - 1 = 93 OCRA)
		
	TIMSK0 |= (1 << OCIE0A); // set ISR COMPA vector
	
	// TIMER 2 - PWM timer (2ms)
	// ps = 1, NM -> 16us = 0.016ms
	// ps = 8, NM -> 128us = 0.128ms
	// ps = 64, NM -> 1024us = 1.024ms
	// ps = 128, NM -> 2048us = 2.048ms **
	// ps = 256, NM -> 4096us = 4.096ms 
	// ps = 1024, NM -> 16384us = 16.384ms
	
	TCCR2A |= (1 << WGM21); // CTC Mode
	
	OCR2A = 249; // timer period (2ms/2.048ms * 256 time steps = 250 time steps - 1 = 249 OCRA)
	OCR2B = 124; // duty cycle

	TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B); // set ISR COMPA and COMPB vector

	TCCR0B |= (1 << CS02) | (1 << CS00); // Pre-scaler set to 1024, STARTS timer0
	TCCR2B |= (1 << CS22) | (1 << CS20); // Pre-scaler set to 128, STARTS timer2
}


int main(void) {
	init(); // initialize inputs and outputs
	GPIO_init(); // initialize GPIO keypad
	timer_init(); // timer settings
	
	sei(); // enable global interrupts

    while (1) {	
		switch(button) {
			case 0: // 0% duty cycle
				OCR2B = 0; // 0
				break;
			case 1: // 10% duty cycle
				OCR2B = 24; // 24
				break;
			case 2: // 20% duty cycle
				OCR2B = 49; // 49
				break;
			case 3: // 30% duty cycle
				OCR2B = 74; // 74
				break;
			case 4: // 40% duty cycle
				OCR2B = 99; // 99
				break;
			case 5: // 50% duty cycle
				OCR2B = 124; // 124
				break;
			case 6: // 60% duty cycle
				OCR2B = 149; // 149
				break;
			case 7: // 70% duty cycle
				OCR2B = 174; // 174
				break;
			case 8: // 80% duty cycle
				OCR2B = 199; // 199
				break;
			case 9: // 90% duty cycle
				OCR2B = 224; // 224
				break;
			default:
				break;
		}
    }
}

ISR (TIMER0_COMPA_vect) { // Timer0 compare register A interrupt
	/* iterative scan of GPIO keypad */
	for (char i = 4; i < 8; ++i) {
		// set row[i] low
		PORTD &= ~(1<<i);
		for (char j = 0; j < 4; ++j) {
			// check column[j] if low
			if ( !(PINB & (1<<j)) ) { // first transition low (on)
				button = buttonmap[i-4][j]; // set button to mapped value
			}
		}
		// set row[i] high
		PORTD |= (1<<i);
	}
}

ISR (TIMER2_COMPA_vect) { // Timer2 compare register A interrupt
	if (button != 0) PORTB |= (1 << PORTB5); // HIGH, LED ON	
}

ISR (TIMER2_COMPB_vect) { // Timer2 compare register B interrupt
	PORTB &= ~(1 << PORTB5); // LOW, LED OFF
}
