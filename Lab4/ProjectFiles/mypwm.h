#ifndef __PWM_H__
#define __PWM_H__


// Variáveis com tamanho padrão
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;



// Defines GPIO
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
// Usando pino G1 para PWM
#define GPIO_RCGC_PORTG 64
#define GPIO_PORTG_BITS 2

#define GPIO_PORTG_AHB_AMSEL_R  (*((volatile uint32_t *)0x4005E528))
#define GPIO_PORTG_AHB_PCTL_R   (*((volatile uint32_t *)0x4005E52C))
#define GPIO_PORTG_AHB_DIR_R    (*((volatile uint32_t *)0x4005E400))
#define GPIO_PORTG_AHB_AFSEL_R  (*((volatile uint32_t *)0x4005E420))
#define GPIO_PORTG_AHB_DEN_R    (*((volatile uint32_t *)0x4005E51C))

// Defines PWM
#define SYSCTL_RCGCPWM_R        (*((volatile uint32_t *)0x400FE640))
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))


#define PWM0_CC_R               (*((volatile uint32_t *)0x40028FC8))
#define PWM0_2_CTL_R            (*((volatile uint32_t *)0x400280C0))
#define PWM0_2_GENB_R           (*((volatile uint32_t *)0x400280E4))
#define PWM0_2_LOAD_R           (*((volatile uint32_t *)0x400280D0))
#define PWM0_2_CMPB_R           (*((volatile uint32_t *)0x400280DC))
#define PWM0_ENABLE_R           (*((volatile uint32_t *)0x40028008))




/****************************************************
 * 
 * Funções do driver.
 *
*****************************************************/
extern void PWM_Init(void);
extern void setDutyCycle(float dutyCycle);
extern void PWM_Enable(void);

#endif
