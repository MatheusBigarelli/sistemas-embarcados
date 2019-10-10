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

#define LADDER_BASE_PIXEL (FLOOR_BASE_PIXEL+FLOOR_HEIGHT)


#define LADDER_SIG_OFFSET 12

#define LADDER_SIG_PRINT_0 (0x01 << LADDER_SIG_OFFSET)
#define LADDER_SIG_PRINT_1 (0x01 << LADDER_SIG_OFFSET+1)
#define LADDER_SIG_PRINT_2 (0x01 << LADDER_SIG_OFFSET+2)
#define LADDER_SIG_PRINT_3 (0x01 << LADDER_SIG_OFFSET+3)

typedef struct {
	int16_t x, y;
} Ladder;

int16_t LADDER_Y(int16_t X);
void drawLadder(Ladder ladder);
void LadderFunc(void);

#endif