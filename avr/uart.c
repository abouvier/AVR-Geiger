#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <avr/io.h>
#include <util/setbaud.h>    // macros for baud rate calculations

#include "uart.h"

#ifdef UDR1
#define UBRRH UBRR1H
#define UBRRL UBRR1L
#define UCSRA UCSR1A
#define UCSRB UCSR1B
#define UCSRC UCSR1C
#define U2X   U2X1
#define UCSZ0 UCSZ10
#define UCSZ1 UCSZ11
#define RXEN  RXEN1
#define TXEN  TXEN1
#define UDRE  UDRE1
#define UDR   UDR1
#endif

#ifdef UDR0
#define UBBRH UBBR0H
#define UBBRL UBBR0L
#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define U2X   U2X0
#define UCSZ0 UCSZ00
#define UCSZ1 UCSZ01
#define RXEN  RXEN0
#define TXEN  TXEN0
#define UDRE  UDRE0
#define UDR   UDR0
#endif

void uart_init(void) {
	UBRRH = UBRRH_VALUE;    // high rate from setbaud.h
	UBRRL = UBRRL_VALUE;    // low rate from setbaud.h

	#if USE_2X
		UCSRA |= _BV(U2X);
	#else
		UCSRA &= ~_BV(U2X);
	#endif

	UCSRC |= _BV(UCSZ1) | _BV(UCSZ0);    // 8 bits
	UCSRB |= _BV(TXEN) | _BV(RXEN);      // enable RX/TX
}

void uart_send(uint16_t num) {
	uint8_t *p = (uint8_t*)&num;           // split num into bytes
	loop_until_bit_is_set(UCSRA, UDRE);    // wait for UDR
	UDR = p[0];                            // send first byte
	loop_until_bit_is_set(UCSRA, UDRE);    // wait for UDR
	UDR = p[1];                            // send last byte
	loop_until_bit_is_set(UCSRA, UDRE);    // wait for UDR
	UDR = '\r';                            // send line break
}