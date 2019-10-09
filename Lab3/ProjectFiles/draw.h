#ifndef __DRAW__H
#define __DRAW__H

#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"
#include "util.h"



void initMap(void);
void draw(const uint8_t img[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i);
void drawEddie(uint16_t xOffset, uint8_t areaOffset);
void drawEnemy(uint16_t xOffset, uint8_t areaOffset, uint8_t extraHeight);
void drawSneaker(uint16_t xOffset, uint8_t areaOffset);
void drawBoss(uint16_t xOffset, uint8_t areaOffset);
void drawItem(uint16_t xOffset, uint8_t areaOffset);

void drawFloor(void);
void drawLadder(void);
void drawScore(void);




#endif
