#ifndef __SIGGEN__
#define __SIGGEN__

#include <math.h>

#include "thread.h"

#include "util.h"

#include "mypwm.h"

#define N 65536.0

float updateDutyCycle(uint8_t waveform, uint16_t n, uint16_t k);

extern void makeSinusoid(void);
extern void makeSawTooth(void);
extern void makeSquare(void);
extern void makeTriangular(void);

#endif
