#ifndef __SIGGEN__
#define __SIGGEN__

#include <math.h>

#include "thread.h"

#include "util.h"

#include "mypwm.h"

#define WAVE_FREQUENCY(X) (20.0*X/3.35)
#define SIG_UPDATE_PWM 0x00000001



extern void makeSinosoid(void);
extern void makeSawTooth(void);
extern void makeSquare(void);
extern void makeTriangular(void);

#endif
