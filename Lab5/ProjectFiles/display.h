#ifndef __DISPLAY__
#define __DISPLAY__

#include <stdio.h>
#include <math.h>

#include "util.h"

#include "thread.h"

#include "grlib/grlib.h"

#include "cfaf128x128x16.h"


#define NUM_DIGITS 3

/**--------------------------------------------------
 * Protótipos das funções.
 *---------------------------------------------------*/
extern void displayInit();
extern void printHeader();
extern void updateInfo(Display_Info *info);



#endif
