#define F_CPU 16000000UL
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD)-1)

#include <avr/io.h>
#include "uart.h"

void uart_init(void){
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_char(char c){
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart_print(const char* str){
	while(*str)
		uart_send_char(*str++);
}
