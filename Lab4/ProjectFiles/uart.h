/**
 * Arquivo .h para device driver da UART
 *
 * Matheus Bigarelli
 * Victor Belinello
 */

#ifndef __UART__
#define __UART__

#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#include "mem_pool.h"

//*****************************************************************************
//
// System Control registers (SYSCTL)
//
//*****************************************************************************
#define SYSCTL_RCGCUART_R (*((volatile uint32_t *)0x400FE618))
#define SYSCTL_PRUART_R (*((volatile uint32_t *)0x400FEA18))



//*****************************************************************************
//
// UART registers (UART0)
//
//*****************************************************************************
#define UART0_DR_R          (*((volatile uint32_t *)0x4000C000))
#define UART0_RSR_R         (*((volatile uint32_t *)0x4000C004))
#define UART0_ECR_R         (*((volatile uint32_t *)0x4000C004))
#define UART0_FR_R          (*((volatile uint32_t *)0x4000C018))
#define UART0_ILPR_R        (*((volatile uint32_t *)0x4000C020))
#define UART0_IBRD_R        (*((volatile uint32_t *)0x4000C024))
#define UART0_FBRD_R        (*((volatile uint32_t *)0x4000C028))
#define UART0_LCRH_R        (*((volatile uint32_t *)0x4000C02C))
#define UART0_CTL_R         (*((volatile uint32_t *)0x4000C030))
#define UART0_IFLS_R        (*((volatile uint32_t *)0x4000C034))
#define UART0_IM_R          (*((volatile uint32_t *)0x4000C038))
#define UART0_RIS_R         (*((volatile uint32_t *)0x4000C03C))
#define UART0_MIS_R         (*((volatile uint32_t *)0x4000C040))
#define UART0_ICR_R         (*((volatile uint32_t *)0x4000C044))
#define UART0_DMACTL_R      (*((volatile uint32_t *)0x4000C048))
#define UART0_9BITADDR_R    (*((volatile uint32_t *)0x4000C0A4))
#define UART0_9BITAMASK_R   (*((volatile uint32_t *)0x4000C0A8))
#define UART0_PP_R          (*((volatile uint32_t *)0x4000CFC0))
#define UART0_CC_R          (*((volatile uint32_t *)0x4000CFC8))

//*****************************************************************************
//
// NVIC registers (NVIC)
//
//*****************************************************************************
#define NVIC_EN0_R              (*((volatile uint32_t *)0xE000E100))
#define NVIC_PRI1_R             (*((volatile uint32_t *)0xE000E404))


// Portas utilizadas no programa
#define UART_ALL_PORTS 0x01


extern void UART_init(void);
extern unsigned char UART0_RxChar(void);
extern void UART0_TxChar(char data);
extern void UART0_TxString(char *data);
extern void UART0_PrintMenu(ID menuID);
void clearUART(void);

#endif
