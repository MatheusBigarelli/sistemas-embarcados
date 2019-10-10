#ifndef __EDDIE__H
#define __EDDIE__H

#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"
#include "buttons.h"
#include "joy.h"

#include "floor.h"
#include "ladder.h"
#include "enemy.h"

#include "collision.h"

#define INIT_AIR_TIME 27

#define DIR_RIGHT true
#define DIR_LEFT  false

#define EDDIE_WIDTH 8
#define EDDIE_HEIGHT 12
#define EDDIE_PIXELS (EDDIE_WIDTH*EDDIE_HEIGHT)

#define EDDIE_SPEED 2
#define EDDIE_JUMP_SPEED 1

// Free space between Eddie and the roof.
#define FREE_SPACE (21-FLOOR_HEIGHT - EDDIE_HEIGHT)

#define EDDIE_BASE_X (64-EDDIE_WIDTH/2)
#define EDDIE_BASE_Y (FLOOR_BASE_PIXEL+21*3+(21-EDDIE_HEIGHT))


typedef enum {
	ON_GROUND,
	HIGH,
	ROOF
} JumpStates;

typedef struct {
	uint8_t id;
	int16_t x, y;
	int16_t last_x, last_y;
	int16_t dx, dy;
	uint8_t jump_state;
	bool last_face_direction;
} Eddie;

void drawEddie(Eddie eddie);
void clearTrace(Eddie eddie);
void deleteXTrace(Eddie eddie);
void deleteYTrace(Eddie eddie);
void EddieThread(void const *args);

#endif