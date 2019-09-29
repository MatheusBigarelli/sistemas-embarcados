#ifndef __ENEMY__H
#define __ENEMY__H

#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"


void drawEnemy(tContext sContext, int16_t x, int16_t y);
void Enemy(tContext sContext);


#endif