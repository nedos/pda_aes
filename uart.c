#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

int uart_putchar(char c, FILE *stream) {

    if (c == '\n') uart_putchar('\r', stream);
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;

    return 0;
}

unsigned char uart_getchar(void) {
    unsigned char c;
    loop_until_bit_is_set(UCSR0A, RXC0);

    c = UDR0;

    if (c == '\r') c = '\n';

    return c;
}

void uart_init(void) {
    UBRR0H = MYUBRR >> 8;
    UBRR0L = MYUBRR;

    // Enable send and recieve
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Set UART to 8N1
    UCSR0C = (0 << USBS0) | (3 << UCSZ00);

    fdevopen(uart_putchar, NULL);
}

void uart_echo(void) {
    unsigned char c;
    for(;;) {
       c = uart_getchar();
       printf("%c",c);
    }
}
