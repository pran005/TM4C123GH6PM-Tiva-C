#ifndef _UART_H
#define _UART_H

#include <inttypes.h>

void init_UART1(void) ; 
void init_UART0(void) ;
void UART0_Tx(char c) ; 
void UART0_Tx(int32_t m) ;
void UART0_Tx(float d);
void UART0_Tx(char * string);
void UART1_Tx(char c);
void UART1_Tx(int32_t m) ; 
void UART1_Tx(float d) ;
void UART1_Tx(char * string) ;   

#endif
