#ifndef __ENEMY__H
#define __ENEMY__H

#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"

#include "floor.h"

#define LEFT_FOOT_UP true
#define RIGHT_FOOT_UP false
	
#define NUM_ENEMIES 2

#define ENEMY_WIDTH 10
#define ENEMY_HEIGHT 6

#define ENEMY_PIXELS (ENEMY_HEIGHT*ENEMY_WIDTH)


void drawEnemy(int16_t x, int16_t y, int16_t last_x, int16_t last_y, bool feet_state);
void Enemy(void const *args);


#endif