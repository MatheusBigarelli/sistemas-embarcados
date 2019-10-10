#ifndef __FLOOR__H
#define __FLOOR__H

#include "cmsis_os.h"
#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"

#define NUM_FLOORS 5

#define FLOOR_WIDTH 128
#define FLOOR_HEIGHT 3
#define FLOOR_PIXELS (FLOOR_WIDTH*FLOOR_HEIGHT)

#define FLOOR_BASE_PIXEL 45

#define FLOOR_SIZE 20


int16_t FLOOR(int16_t X);
void drawFloor(uint8_t base_height);
void Floor(void);

#endif