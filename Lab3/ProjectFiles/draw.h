#ifndef __DRAW__H
#define __DRAW__H

#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"
#include "util.h"
#include "images.h"

#define MAP_HEIGHT 128
#define MAP_WIDTH 128



extern tContext sContext;

enum dir
{
    RIGHT = 1, // No caso do eixo Y RIGHT significa para cima
    LEFT = -1, // No caso do eixo Y LEFT significa para baixo
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

struct image
{
	ColorIndex colorIndex;
	uint16_t width, height;
	uint16_t x,y;
	uint8_t* data;
	Direction dirX, dirY;
	uint8_t areaOffset;
	bool needsUpdate, isMoving;
};
typedef struct image Image;

void initMap(void);
void draw2(Image img);
void draw(const uint8_t img[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i, Direction dir, ColorIndex index);
void checkColision(ColorIndex index1, ColorIndex index2, uint16_t areaOffset);

// As funcoes abaixo estao definidas nos respectivos .c e nao no draw.c
void drawEddie(Image img);

void drawEnemy(Image img, uint8_t extraHeight);
void drawSneaker(Image img);
void drawBoss(Image img);

void drawItem(Image img);

void drawFloor(Image img);
void drawLadder(Image img);
void drawScore(void);




#endif
