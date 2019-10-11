#include "draw.h"

extern uint8_t head[] ;
extern uint8_t legs[];
extern uint8_t legsExtra[];

void drawSneaker(uint16_t xOffset, uint8_t areaOffset, Direction dir)
{
	drawEnemy(xOffset, areaOffset, 0, dir);
}
void drawBoss(uint16_t xOffset, uint8_t areaOffset, Direction dir)
{
	drawEnemy(xOffset, areaOffset, 5, dir);
}

void drawEnemy(uint16_t xOffset, uint8_t areaOffset, uint8_t extraHeight, Direction dir)
{
	int i, j = 0, sneakerHeight, headTopOffset;

	sneakerHeight = HEAD_HEIGHT + LEGS_HEIGHT + extraHeight;
	headTopOffset = (127 - FLOOR_HEIGHT) - sneakerHeight - (LADDER_HEIGHT + FLOOR_HEIGHT) * (areaOffset);
	// draw(head, HEAD_HEIGHT, HEAD_WIDTH, xOffset, headTopOffset,dir,ENEMY_HEAD);
	for (i = 0; i < extraHeight; i++)
	{
		// draw(legsExtra, LEGS_EXTRA_HEIGHT, LEGS_EXTRA_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + i,dir,ENEMY_LEGS);
	}
	flipVert(legs, LEGS_HEIGHT, LEGS_WIDTH);
	// draw(legs, LEGS_HEIGHT, LEGS_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + extraHeight,dir,ENEMY_LEGS);
}