#ifndef __UTIL__H
#define __UTIL__H
#include <stdint.h>

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1us(uint32_t delay);
void DisableInterrupts(void);
void EnableInterrupts(void);


#endif 
