/*
 * Lab5.c
 *
 * Created: 10/4/2024 9:53:03 PM
 * Author : rnsot
 */ 


#define F_CPU 16000000UL // 16MHz clock speed

#include <avr/io.h>

void init(void) {
	DDRC |= (1 << DDRC5); // output pin PC5
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

void timer_init(void) { // initialize timer settings
	// FCPU = 16MHz, want (max) 2.273ms 
		// ps = 1, NM -> 16us = 0.016ms
		// ps = 8, NM -> 128us = 0.128ms
		// ps = 64, NM -> 1024us = 1.024ms
		// ps = 256, NM -> 4096us = 4.096ms **
		// ps = 1024, NM -> 16384us = 16.384ms

	/*
	want PERIOD given PS = x256:
	calculate wanted period per note (1/(440*2^(x/12)))
	 x | us*10     us	   ms  steps   -1 
A4	 0 | 22727 = 2273 = 2.273 = 142 = 141
A#4	 1 | 21452 = 2145 = 2.145 = 134 = 133
B4	 2 | 20248 = 2025 = 2.025 = 127 = 126
C5	 3 | 19111 = 1911 = 1.911 = 119 = 118
C#5	 4 | 18039 = 1804 = 1.804 = 113 = 112
D5	 5 | 17026 = 1703 = 1.703 = 106 = 105
D#5	 6 | 16071 = 1607 = 1.607 = 100 =  99
E5	 7 | 15169 = 1517 = 1.517 =  95 =  94
F5	 8 | 14317 = 1432 = 1.432 =  89 =  88
F#5	 9 | 13514 = 1351 = 1.351 =  84 =  83
G5	10 | 12755 = 1276 = 1.276 =  80 =  79
G#5	11 | 12039 = 1204 = 1.204 =  75 =  74
A5	12 | 11364 = 1136 = 1.136 =  71 =  70
A#5	13 | 10726 = 1073 = 1.073 =  67 =  66
B5	14 | 10124 = 1012 = 1.012 =  63 =  62
C6	15 | 09556 = 0956 = 0.956 =  60 =  59
	*/
	
	// Set to CTC Mode
	TCCR0A |= (1 << WGM01);
	
	// Pre-scaler set to 256, STARTS the timer
	TCCR0B |= (1 << CS02);
}

void generate_PWM(void) { // generates a PWM waveform with variable duty cycle
	PORTC |= (1 << PORTC5); // HIGH
	while ( (TIFR0 & (1 << OCF0B)) == 0 ) {} // busy while until OCF0B flag == 1 in TIFR0 register
	TIFR0 |= (1 << OCF0B); // reset OCR0B overflow
	
	PORTC &= ~(1 << PORTC5); // LOW
	while ( (TIFR0 & (1 << OCF0A)) == 0 ) {} // busy while until OCF0A flag == 1 in TIFR0 register
	TIFR0 |= (1 << OCF0A); // reset OCR0A overflow flag
}

int main(void) {
    
	init();
	GPIO_init();
	timer_init();
	
	
	unsigned int keynote[4][4] =
	{ // pre-calculated periods
		{141, 133, 126, 118},
		{112, 105, 99, 94},
		{88, 83, 79, 74},
		{70, 66, 62 , 59}
	};
	
	while (1) {
		/* iterative scan of GPIO keypad */
		for (char i = 4; i < 8; ++i) {
			// set row[i] low
			PORTD &= ~(1<<i);
			for (char j = 0; j < 4; ++j) {
				// check column[j] if low
				if ( !(PINB & (1<<j)) ) { // first transition low (on)
					OCR0A = keynote[i-4][j]; // set period (frequency) based on index
					OCR0B = OCR0A >> 1; // 50% duty cycle
					while ( !(PINB & (1<<j)) ) { // wait until button is released
						generate_PWM(); // output at PINC5
					}
				}	
			}
			// set row[i] high
			PORTD |= (1<<i);
		}
    }
	
}

