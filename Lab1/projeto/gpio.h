#include <stdint.h>
#include "tm4c1294ncpdt.h";

#define GPIO_JN 0x1100

void GPIOInit();
uint32_t readPortJ();
void writePortN(uint32_t arg);