#include <stdint.h>
#include "tm4c1294ncpdt.h"

#define GPIO_JN 0x1100

void GPIOInit(void);
uint32_t readPortJ(void);
void writePortN(uint32_t arg);

extern void EnableInterrupts(void);
extern void DisableInterrupts(void);