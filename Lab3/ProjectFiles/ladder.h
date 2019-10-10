#ifndef __LADDER__H
#define __LADDER__H

#include "cmsis_os.h"
#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"

#define LADDER_WIDTH 12
#define LADDER_HEIGHT 17

#define LADDER_PIXELS (LADDER_WIDTH*LADDER_HEIGHT)
#define NUM_LADDERS 4

#define LADDER_BASE_PIXEL (FLOOR_BASE_PIXEL+FLOOR_HEIGHT+1)


int16_t LADDER_Y(int16_t X);
void drawLadder(uint16_t x, uint16_t y);
void Ladder(void);

#endif