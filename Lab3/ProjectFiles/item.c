#include "draw.h"

extern uint8_t item[];

extern uint32_t palette[9];


uint32_t currentColor = 0x00C97ABB;
void drawItem(Image img)
{
	int i, j = 0, itemTopOffset;
	
	itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT) * (img.areaOffset) + 1;
	palette[ITEM] = currentColor;
	img.colorIndex = ITEM;
	img.data = item;
	img.width = ITEM_WIDTH;
	img.height = ITEM_HEIGHT;
	img.y = itemTopOffset;
	draw(img);
	currentColor += 1;
	
}
