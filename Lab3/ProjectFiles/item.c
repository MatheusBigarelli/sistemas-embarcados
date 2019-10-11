#include "draw.h"

extern uint8_t item[];

void drawItem(Image img)
{
	int i, j = 0, itemTopOffset;
	itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT) * (img.areaOffset) + 1 + 10;
	img.colorIndex = ITEM;
	img.data = item;
	img.width = ITEM_WIDTH;
	img.height = ITEM_HEIGHT;
	img.y = itemTopOffset;
	draw2(img);
}
