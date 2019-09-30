#ifndef __EDDIE__H
#define __EDDIE__H

#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"
#include "buttons.h"
#include "joy.h"


void drawEddie(tContext sContext, int16_t x, int16_t y, uint16_t last_x, uint16_t last_y);
void Eddie(tContext sContext);

#endif