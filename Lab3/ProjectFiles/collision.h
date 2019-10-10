#ifndef __COLLISION__
#define __COLLISION__

#include "eddie.h"
#include "floor.h"
#include "ladder.h"
#include "item.h"
#include "enemy.h"

typedef enum {
	ID_EDDIE,
	ID_ENEMY,
	ID_ITEM,
	ID_LADDER
} Character;

typedef struct {
	int16_t x, y;
	int16_t width, height;
} Object;

#endif