#ifndef __EDDIE__H
#define __EDDIE__H

#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"
#include "buttons.h"
#include "joy.h"

#include "floor.h"

#define INIT_AIR_TIME 20

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


void drawEddie(int16_t x, int16_t y, int16_t last_x, int16_t last_y, bool last_face_direction);
void clearTrace(int16_t x, int16_t y, int16_t last_x, int16_t last_y);
void deleteXTrace(int16_t x, int16_t last_x, int16_t y, int16_t last_y);
void deleteYTrace(int16_t y, int16_t last_y, int16_t x, int16_t last_x);
void Eddie(void const *args);

#endif