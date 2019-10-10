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
	
#define NUM_ENEMIES 3

#define ENEMY_WIDTH 8
#define ENEMY_HEIGHT 5

#define ENEMY_PIXELS (ENEMY_HEIGHT*ENEMY_WIDTH)

typedef struct {
	int16_t x, y;
	int16_t last_x, last_y;
	int16_t speed;
	bool feet_state;
} Enemy;


int16_t ENEMY_Y(int16_t X);
void drawEnemy(Enemy enemy);
void EnemyThread(void const *args);


#endif