/*
 * f3d_uart.h
 *
 * Created on: 02/11/2016
 * Author: Shichao Hu
 * Partner: Bradmore Tung
 * Last Modified: 4/6/2016
 * Part of: Lab5
 *
 */

void f3d_uart_init(void);
int putchar(int);
int getchar(void);
void putstring(char *);

//// flush uart
void flush_uart(void);

/* f3d_uart.h ends here */
