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

#define INIT_AIR_TIME 20

#define DIR_RIGHT true
#define DIR_LEFT  false

#define EDDIE_WIDTH 6
#define EDDIE_HEIGHT 15

#define EDDIE_SPEED 2


void drawEddie(int16_t x, int16_t y, int16_t last_x, int16_t last_y, bool last_face_direction);
void clearTrace(int16_t x, int16_t y, int16_t last_x, int16_t last_y);
void deleteXTrace(int16_t x, int16_t last_x, int16_t y, int16_t last_y);
void deleteYTrace(int16_t y, int16_t last_y, int16_t x, int16_t last_x);
void Eddie(void const *args);

#endif