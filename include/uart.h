#ifndef UART_H_
#define UART_H_

void uart_init(void);
void uart_send_char(char c);
void uart_print(const char* str);

#endif
