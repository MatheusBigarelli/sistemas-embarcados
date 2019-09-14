#ifndef __UART__H
#define __UART__H

#include <stdint.h>
#include <string.h>
#include "tm4c1294ncpdt.h"

#define UART_ALL_PORTS 0x01

void UART_Init(void);
unsigned char UART0_RxChar(void);
void UART0_TxChar(unsigned char data);
void UART0_TxString(unsigned char* data);
#endif
