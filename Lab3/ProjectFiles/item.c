#include "draw.h"

extern uint8_t item[];

extern uint32_t palette[9];

extern char buffer[2][MAP_HEIGHT][MAP_WIDTH];

void collectItem(Image* item)
{
	int i,j;
	for (i = 0; i < item->height; i++)
	{
		for (j = 0; j < item->width; j++)
		{
				GrContextForegroundSet(&sContext, ClrBlack);
				GrPixelDraw(&sContext, item->x + j, i + item->y);
				buffer[0][i + item->y][item->x + j] = ClrBlack;		
		}
	}
	item->needsUpdate = true;
	item->isMoving = false;
	item->collected = true;
}
void updateItens(Image* item1, Image* item2)
{
	int k;
	if(item1->collected == false)
	{
		if(item1->x == 0)
		{
			clear(*item1);
			item1->x = 127;
		}
		if(item1->x == 128)
		{
			item1->x = 0;
		}
		if(item1->dirX == RIGHT)
		{
			item1->x++;
		}
		if(item1->dirX == LEFT)
		{
			item1->x--;
		}
	}
	else
	{
		clear(*item1);
		item1->x = 0;
		for (k = -NUMBER_OF_AREAS + 1; k < NUMBER_OF_AREAS; k++)
		{
			if(k == 0) continue;
			if(item1->areaOffset + k != item2->areaOffset && item1->areaOffset + k < NUMBER_OF_AREAS && item1->areaOffset + k >= 0)
			{
				item1->areaOffset += k;
				break;
			}
		}	
		item1->collected = false;	
	}
	if(item2->collected == false)
	{
		if(item2->x == 0)
		{
			clear(*item2);
			item2->x = 127;
		}
		if(item2->x == 128)
		{
			item2->x = 0;
		}
		if(item2->dirX == RIGHT)
		{
			item2->x++;
		}
		if(item2->dirX == LEFT)
		{
			item2->x--;
		}
	}
	else
	{
		clear(*item2);
		item2->x = 0;
		for (k = -NUMBER_OF_AREAS + 1; k < NUMBER_OF_AREAS; k++)
		{
			if(k == 0) continue;
			if(item2->areaOffset + k != item1->areaOffset && item2->areaOffset + k < NUMBER_OF_AREAS && item2->areaOffset + k >= 0)
			{
				item2->areaOffset += k;
				break;
			}
		}
		item2->collected = false;		
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
