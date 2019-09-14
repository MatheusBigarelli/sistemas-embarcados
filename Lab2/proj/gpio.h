#ifndef __GPIO__H
#define __GPIO__H

#include <stdint.h>
#include "tm4c1294ncpdt.h"

#define GPIO_ALL_PORTS	0x1131

#define GPIO_PORTJ_BITS	0x01
#define GPIO_PORTA_BITS 0x03
#define GPIO_PORTE_BITS 0x03
#define GPIO_PORTF_BITS 0x02

#define GPIO_PORTN_BITS 0x01


void GPIO_Init(void);
uint32_t PortJ_Input(void);
void PortE_Output(uint32_t value);
void PortF_Output(uint32_t value);

void PortN_Output(uint32_t value);
#endif
