#ifndef __SIGGEN__
#define __SIGGEN__

#include <math.h>


#include "util.h"
#include "mypwm.h"

#define WAVE_FREQUENCY(X) (20.0*X/3.35)



extern void makeSinosoid(void);
void makeSawTooth(void);

#endif
