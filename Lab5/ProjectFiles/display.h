#ifndef __DISPLAY__
#define __DISPLAY__

#include <math.h>

#include "util.h"

#include "thread.h"

#include "grlib/grlib.h"

#include "cfaf128x128x16.h"

/**--------------------------------------------------
 * Protótipos das funções.
 *---------------------------------------------------*/
extern void displayInit();
extern void printHeader();

extern void floatToString(float value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros, uint8_t precision);
extern void intToString(int64_t value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros);


#endif
