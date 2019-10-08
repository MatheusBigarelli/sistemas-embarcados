#ifndef __ENEMY__H
#define __ENEMY__H

#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"


void drawEnemy(int16_t x, int16_t y, int16_t last_x, int16_t last_y);
void Enemy(void const *args);


#endif