/*
 * Lab3.c
 *
 * Created: 9/19/2024 3:38:44 PM
 * Author : rnsot
 */ 


#define F_CPU 16000000UL // 16MHz clock speed

#define BAUD 9600 // bits/sec
#define BAUDRATE ((F_CPU)/(BAUD*16UL) - 1)

#include <avr/io.h>


void usart_init (void) { // initialize AVR settings
	//UBRR0H &= 0; // clear high bits,
	UBRR0H = (BAUDRATE>>8); // shift register right by 8 bits
	UBRR0L = BAUDRATE; // set baud rate
	
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0); // enable receiver and transmitter
	// only transmitter is used for this lab
	
	UCSR0C &= ~(1<<UMSEL00) | ~(1<<UMSEL01); // Asynchronous USART mode
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01); // 8 data bit frame
	UCSR0C &= ~(1<<UPM01) | ~(1<<UPM00); // disable parity bit
	UCSR0C &= ~(1<<USBS0); // 1 stop bit
}

void usart_transmit(char data) { // transmit one char
	while ( !(UCSR0A & (1<<UDRE0)) ); // wait until transmit buffer is empty
	UDR0 = data; // write data to register
}

char usart_receive(void) { // receive one char
	while ( !(UCSR0A & (1<<RXC0)) ); // wait while data is received
	return UDR0; // return data
}


int main(void) {
	
	// map row/column to key
	char keychar[4][4] = 
		{
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#' , 'D'} 
		};
	 
	
	/* initialization */
	usart_init();
	// Rows = PORTD, Columns = PORTB
	// set rows to output
	DDRD |= (1<<DDD4 | 1<<DDD5 | 1<<DDD6 | 1<<DDD7); // Port D (4-7)
	// set column to input
	DDRB &= ~(1<<DDB0 | 1<<DDB1 | 1<<DDB2 | 1<<DDB3); // Port B (0-3)
	// pull up property enabled (columns)
	PORTB |= (1<<PORTB0 | 1<<PORTB1 | 1<<PORTB2 | 1<<PORTB3);
	// rows to logic high
	PORTD |= (1<<PORTD4 | 1<<PORTD5 | 1<<PORTD6 | 1<<PORTD7);
	
    while (1) {
		/* iterative scan */
		for (char i = 4; i < 8; ++i) {
			// set row[i] low
			PORTD &= ~(1<<i);
			for (char j = 0; j < 4; ++j) {
				// check column[j] if low
				if ( !(PINB & (1<<j)) ) { // first transition low (on)
					usart_transmit(keychar[i-4][j]);
					while ( !(PINB & (1<<j)) ) {} // wait until button is released
				}
				
			}
			// set row[i] high
			PORTD |= (1<<i);
		}		
    }
	
}

