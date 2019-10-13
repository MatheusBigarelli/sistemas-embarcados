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

#define NUMBER_OF_AREAS 4
#define NUMBER_OF_LADDERS_IN_AREA 2

extern tContext sContext;

enum dir
{
	UP = 2,
	DOWN = -2,
	RIGHT = 1, // UP qnd pulando
	LEFT = -1, // DOWN qnd  pulando
	NONE = 0
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
	bool needsUpdate, isMoving, collected;
};
typedef struct image Image;

void initMap(void);
void clear(Image img);
void clearEddie(Image eddie);
void draw(Image img);
void checkColision(ColorIndex index1, ColorIndex index2, uint16_t areaOffset);
void blinkDisplay(void);

// As funcoes abaixo estao definidas nos respectivos .c e nao no draw.c
void drawEddie(Image img);

void drawEnemy(Image img, uint8_t extraHeight);
void drawSneaker(Image img);
void drawBoss(Image img);

void drawItem(Image* img);

void drawFloor(Image img);
void drawLadder(Image img);
void drawScore(void);

Direction eddieCanGoToLadder(uint16_t eddieXPosition, uint8_t eddieAreaOffset);
void handleEddieJump(Image* eddie);
void handleEddieMovement(Image* eddie);

void updateItens();
void collectItem(Image* item);
void replaceItem(void const* arg);
void clearPreviousScore();
#endif
