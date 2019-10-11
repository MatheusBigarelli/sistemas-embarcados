#include "draw.h"

extern uint8_t ladder[];

extern uint8_t floor[];



void drawLadder(Image img)
{
	int i, j = 0;
	int ladderStarts[NUMBER_OF_AREAS][NUMBER_OF_LADDERS_IN_AREA] = {
		{10, 70},
		{60, 30},
		{0, 50},
		{100, 20}};
	img.colorIndex = LADDER;
	img.width = LADDER_WIDTH;
	img.height = LADDER_HEIGHT;
	if(img.needsUpdate)
	{
		img.data = ladder;
		for (i = 0; i < NUMBER_OF_AREAS; i++)
		{
			img.y = (127 - FLOOR_HEIGHT) - LADDER_HEIGHT * (i + 1) - FLOOR_HEIGHT * i;
			for (j = 0; j < NUMBER_OF_LADDERS_IN_AREA; j++)
			{
				img.x = ladderStarts[i][j];
				draw(img);
			}
		}
	}
	
}

void drawScore(void)
{
	int i, j, lifes = 3;
	int lifeIndicatorLenght = 8;
	int start;
	GrContextForegroundSet(&sContext, ClrWhite);
	GrStringDrawCentered(&sContext, "00000", -1, 64, (sContext.psFont->ui8Height) / 2, true);
	for (i = 0; i < lifes; i++)
	{
		start = 48 + i * (lifeIndicatorLenght + 3);
		for (j = 0; j < lifeIndicatorLenght; j++)
		{
			GrPixelDraw(&sContext, start + j, (sContext.psFont->ui8Height) + 2);
		}
	}
}

void drawFloor(Image img)
{
	int i, j = 0;
	int numberOfFloors = 5;

	img.colorIndex = FLOOR;
	img.width = FLOOR_WIDTH;
	img.height = FLOOR_HEIGHT;
	img.x = 0;
	if(img.needsUpdate)
	{
		img.data = floor;
		for (i = 0; i < numberOfFloors; i++)
		{
			img.y =  (127 - FLOOR_HEIGHT) - i * (FLOOR_HEIGHT + LADDER_HEIGHT);
			draw(img);
		}
	}
	
}