#include "draw.h"

extern uint8_t item[];

void drawItem(uint16_t xOffset, uint8_t areaOffset, Direction dir)
{
	int i, j = 0, itemTopOffset;
	itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT) * (areaOffset) + 1 + 10;
	// draw(item, ITEM_HEIGHT, ITEM_WIDTH, xOffset, itemTopOffset,dir,ITEM);
}
