#include "draw.h"

extern uint8_t ladder[];

extern uint8_t floor[];

void drawLadder(void)
{
	int i, j = 0;

	int numberOfAreas = 4;
	int numberOfLaddersInArea = 2;

	int ladderStarts[4][2] = {
		{10, 70},
		{60, 30},
		{0, 50},
		{100, 20}};

	for (i = 0; i < numberOfAreas; i++)
	{
		for (j = 0; j < numberOfLaddersInArea; j++)
		{
			// draw(ladder, LADDER_HEIGHT, LADDER_WIDTH, ladderStarts[i][j], (127 - FLOOR_HEIGHT) - LADDER_HEIGHT * (i + 1) - FLOOR_HEIGHT * i,NONE,LADDER);
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

void drawFloor(void)
{
	int i, j = 0;
	int numberOfFloors = 5;
	
	for (i = 0; i < numberOfFloors; i++)
	{
		// draw(floor, FLOOR_HEIGHT, FLOOR_WIDTH, 0, (127 - FLOOR_HEIGHT) - i * (FLOOR_HEIGHT + LADDER_HEIGHT),NONE,FLOOR);
	}
}