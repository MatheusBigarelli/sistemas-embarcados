#ifndef __DRAW__H
#define __DRAW__H

#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"


void initMap();
void draw(tContext sContext, const uint32_t oneChannel[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i, const uint8_t priority);
void Eddie(tContext sContext, uint16_t xOffset, uint8_t areaOffset);
void Sneaker(tContext sContext, uint16_t xOffset, uint8_t areaOffset);
void Floor(tContext sContext);
void Ladder(tContext sContext);

#endif