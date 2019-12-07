/**
 * Arquivo .h para device driver da UART
 *
 * Matheus Bigarelli
 * Victor Belinello
 */

#ifndef __UART__
#define __UART__

#include "util.h"
#include "tm4c1294ncpdt.h"

#define GPIO_PORTA_BITS 0x03
#define GPIO_ALL_PORTS GPIO_PORTA_BITS
extern void GPIO_Init(void);

// Portas utilizadas no programa
#define UART_ALL_PORTS 0x01


extern void UART_init(void);
extern unsigned char UART0_RxChar(void);
extern void UART0_TxChar(char data);
extern void UART0_TxString(char *data);

void clearUART(void);
void printGanttDiagram(void);

bool checkChar(char c);
bool isCommand(char *buffer, int size);
void clearBuffer(char *buffer, int *size);

#endif