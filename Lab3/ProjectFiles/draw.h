#ifndef __DRAW__H
#define __DRAW__H

#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"
#include "util.h"

enum dir
{
    NONE,
    RIGHT,
    LEFT,
	UPDATE //desculpa nao quero colocar mais argumento na draw
};
typedef enum dir Direction;

enum colors
{
	EMPTY = 0,
	LADDER,
	FLOOR,
	ITEM,
	ENEMY_LEGS,
	ENEMY_HEAD,
	EDDIE_SHIRT,
	EDDIE_HAT,
	EDDIE_BODY
};
typedef enum colors ColorIndex;

enum colisions
{
	NO_COLISION = 0,
	EDDIE_ITEM,
	EDDIE_ENEMY
};
typedef enum colisions TypeOfColision;

void initMap(void);
void draw(const uint8_t img[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i, Direction dir, ColorIndex index);
void clear(ColorIndex index, const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i);
void drawEddie(uint16_t xOffset, uint8_t areaOffset, Direction dir);
void drawEnemy(uint16_t xOffset, uint8_t areaOffset, uint8_t extraHeight, Direction dir);
void drawSneaker(uint16_t xOffset, uint8_t areaOffset, Direction dir);
void drawBoss(uint16_t xOffset, uint8_t areaOffset, Direction dir);
void drawItem(uint16_t xOffset, uint8_t areaOffset, Direction dir);

void drawFloor(void);
void drawLadder(void);
void drawScore(void);

TypeOfColision checkColision(ColorIndex index1, ColorIndex index2);


#endif
