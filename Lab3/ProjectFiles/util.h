#ifndef __UTIL__H
#define __UTIL__H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void flipVert(uint32_t image[], uint16_t rows, uint16_t columns);
void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros);

#endif