#include "draw.h"

extern uint8_t item[];

extern uint32_t palette[9];

void collectItem(Image* item)
{

}
void updateItens(Image* item1, Image* item2)
{
	if(item1->x == 0)
	{
		clear(*item1);
		item1->x = 127;
	}
	if(item2->x == 0)
	{
		clear(*item2);
		item2->x = 127;
	}
	if(item1->x == 128)
	{
		item1->x = 0;
	}
	if(item2->x == 128)
	{
		item2->x = 0;
	}
	if(item1->dirX == RIGHT)
	{
		item1->x++;
	}
	if(item2->dirX == RIGHT)
	{
		item2->x++;
	}
	if(item1->dirX == LEFT)
	{
		item1->x--;
	}
	if(item2->dirX == LEFT)
	{
		item2->x--;
	}
}
uint32_t currentColor = 0x00C97ABB;
void drawItem(Image* img)
{
	int i, j = 0, itemTopOffset;
	
	itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT) * (img->areaOffset);
	palette[ITEM] = currentColor;
	img->colorIndex = ITEM;
	img->data = item;
	img->width = ITEM_WIDTH;
	img->height = ITEM_HEIGHT;
	img->y = itemTopOffset;
	draw(*img);
	currentColor += 1;
	if(currentColor == 0x00FFFFFF)
	{
		currentColor = 0x00C97ABB;
	}
}
