#ifndef __LADDER__H
#define __LADDER__H

#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"

void drawLadder(tContext sContext, uint16_t x, uint16_t y);
void Ladder(tContext sContext);

#endif