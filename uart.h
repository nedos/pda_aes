#define MYUBRR ((F_CPU+(BAUD_RATE*8L))/(BAUD_RATE*16L)-1)


int uart_putchar(char c, FILE *stream);

unsigned char uart_getchar(void);

void uart_init(void);
