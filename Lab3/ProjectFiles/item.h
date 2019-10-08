#ifndef __ITEM__H
#define __ITEM__H

#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"

#define NUM_ITENS 3
#define NUM_CHANNELS 3

#define ITEM_WIDTH 13
#define ITEM_HEIGHT 7

#define ITEM_PIXELS (ITEM_WIDTH*ITEM_HEIGHT*3)

void drawItem(int16_t x, int16_t y, int16_t last_x, int16_t last_y, uint8_t state[NUM_CHANNELS]);
void Item(void const *args);

#endif