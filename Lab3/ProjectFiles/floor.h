#ifndef __FLOOR__H
#define __FLOOR__H

#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"


#define FLOOR_WIDTH 128
#define FLOOR_HEIGHT 3


void drawFloor(tContext sContext);
void Floor(tContext sContext);

#endif