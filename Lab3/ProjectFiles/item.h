#ifndef __ITEM__H
#define __ITEM__H

#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "grlib/grlib.h"
#include "cfaf128x128x16.h"

#include "floor.h"

#define NUM_ITENS 3
#define NUM_CHANNELS 3


#define ITEM_WIDTH 7
#define ITEM_HEIGHT 4
#define ITEM_PIXELS (ITEM_WIDTH*ITEM_HEIGHT)

typedef struct {
	uint8_t id;
	int16_t x, y, speed;
	int16_t last_x, last_y;
	uint8_t glow_state[NUM_CHANNELS];
} Item;


void drawItem(Item item);
void ItemThread(void const *args);
int16_t ITEM_Y(int16_t X);


#endif