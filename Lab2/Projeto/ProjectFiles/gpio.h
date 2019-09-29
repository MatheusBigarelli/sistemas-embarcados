#ifndef __GPIO__H
#define __GPIO__H

#include <stdint.h>
#include "driverlib/gpio.h"
#include "tm4c1294ncpdt.h"

#define GPIO_ALL_PORTS	0x1001
#define GPIO_PORTN_BITS 0x01
#define GPIO_PORTA_BITS 0x03

void GPIO_Init(void);
void PortN_Output(uint32_t value);

#endif