#ifndef __GPIO__H
#define __GPIO__H

#include <stdint.h>

#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_PRGPIO_R (*((volatile uint32_t *)0x400FEA08))

//*****************************************************************************
//
// GPIO registers (PORTA AHB)
//
//*****************************************************************************
#define GPIO_PORTA_AHB_DATA_BITS_R \
    ((volatile uint32_t *)0x40058000)
#define GPIO_PORTA_AHB_DATA_R (*((volatile uint32_t *)0x400583FC))
#define GPIO_PORTA_AHB_DIR_R (*((volatile uint32_t *)0x40058400))
#define GPIO_PORTA_AHB_IS_R (*((volatile uint32_t *)0x40058404))
#define GPIO_PORTA_AHB_IBE_R (*((volatile uint32_t *)0x40058408))
#define GPIO_PORTA_AHB_IEV_R (*((volatile uint32_t *)0x4005840C))
#define GPIO_PORTA_AHB_IM_R (*((volatile uint32_t *)0x40058410))
#define GPIO_PORTA_AHB_RIS_R (*((volatile uint32_t *)0x40058414))
#define GPIO_PORTA_AHB_MIS_R (*((volatile uint32_t *)0x40058418))
#define GPIO_PORTA_AHB_ICR_R (*((volatile uint32_t *)0x4005841C))
#define GPIO_PORTA_AHB_AFSEL_R (*((volatile uint32_t *)0x40058420))
#define GPIO_PORTA_AHB_DR2R_R (*((volatile uint32_t *)0x40058500))
#define GPIO_PORTA_AHB_DR4R_R (*((volatile uint32_t *)0x40058504))
#define GPIO_PORTA_AHB_DR8R_R (*((volatile uint32_t *)0x40058508))
#define GPIO_PORTA_AHB_ODR_R (*((volatile uint32_t *)0x4005850C))
#define GPIO_PORTA_AHB_PUR_R (*((volatile uint32_t *)0x40058510))
#define GPIO_PORTA_AHB_PDR_R (*((volatile uint32_t *)0x40058514))
#define GPIO_PORTA_AHB_SLR_R (*((volatile uint32_t *)0x40058518))
#define GPIO_PORTA_AHB_DEN_R (*((volatile uint32_t *)0x4005851C))
#define GPIO_PORTA_AHB_LOCK_R (*((volatile uint32_t *)0x40058520))
#define GPIO_PORTA_AHB_CR_R (*((volatile uint32_t *)0x40058524))
#define GPIO_PORTA_AHB_AMSEL_R (*((volatile uint32_t *)0x40058528))
#define GPIO_PORTA_AHB_PCTL_R (*((volatile uint32_t *)0x4005852C))
#define GPIO_PORTA_AHB_ADCCTL_R (*((volatile uint32_t *)0x40058530))
#define GPIO_PORTA_AHB_DMACTL_R (*((volatile uint32_t *)0x40058534))
#define GPIO_PORTA_AHB_SI_R (*((volatile uint32_t *)0x40058538))
#define GPIO_PORTA_AHB_DR12R_R (*((volatile uint32_t *)0x4005853C))
#define GPIO_PORTA_AHB_WAKEPEN_R \
    (*((volatile uint32_t *)0x40058540))
#define GPIO_PORTA_AHB_WAKELVL_R \
    (*((volatile uint32_t *)0x40058544))
#define GPIO_PORTA_AHB_WAKESTAT_R \
    (*((volatile uint32_t *)0x40058548))
#define GPIO_PORTA_AHB_PP_R (*((volatile uint32_t *)0x40058FC0))
#define GPIO_PORTA_AHB_PC_R (*((volatile uint32_t *)0x40058FC4))

//*****************************************************************************
//
//
//
//*****************************************************************************

#define GPIO_PORTA_BITS 0x03
#define GPIO_ALL_PORTS GPIO_PORTA_BITS

void GPIO_Init(void);

#endif

