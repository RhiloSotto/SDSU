/*
 * Lab2.c
 *
 * Created: 9/12/2024 3:47:01 PM
 * Author : rnsot
 */ 

#define F_CPU 16000000UL // 16MHz clock speed

#define BAUD 9600 // bits/sec
#define BAUDRATE ((F_CPU)/(BAUD*16UL) - 1) // UBRR


#include <avr/io.h>
#include <util/delay.h> // to use delays

void usart_init (void) { // initialize AVR settings
	UBRR0H = (BAUDRATE>>8); // store 8 most significant bits of BAUDRATE
	UBRR0L = BAUDRATE; // store 8 least significant bits of BAUDRATE
	
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

int main(void) {	
	usart_init();
	char redID[] = "130551574";
	
	uint8_t stringIndex = 0; // iterate through each character
	while (1) {
		if (redID[stringIndex] != '\0') { // until end is reached
			usart_transmit(redID[stringIndex]);
			++stringIndex; // next index
		}
		else {
			usart_transmit('\r'); // carriage return
			usart_transmit('\n'); // line feed
			_delay_ms(500); // 500ms delay
			stringIndex = 0; // back to beginning
		}
		
    }
}

