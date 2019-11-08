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

#define PWM0_ENABLE_M0PWM5       (0x1 << 5) // Macro para identificar o M0PWM5.

#define PWM_X_ENABLE            0x1 // Macro para ativar um módulo de PWM.

#define PWM_PERIOD_MAX          0x10000
#define PWM_PERIOD_MIN          0x00000

#define PWM_SYSCLK_DIV2         0x100
#define PWM_SYSCLK_DIV4         0x101
#define PWM_SYSCLK_DIV8         0x102
#define PWM_SYSCLK_DIV16        0x103
#define PWM_SYSCLK_DIV32        0x104
#define PWM_SYSCLK_DIV64        0x105


/****************************************************
 * 
 * Funções do driver.
 *
*****************************************************/
extern void PWM_Init(void);
extern void setDutyCycle(float dutyCycle);
extern void PWM_Enable(void);
extern void PWM_Disable(void);
extern void setPeriod(uint16_t period);
#endif
