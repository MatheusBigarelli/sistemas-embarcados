#include "draw.h"

#include "floor.h"
#include "sneaker.h"
#include "name.h"
#include "item.h"
#include "eddie.h"
#include "ladder.h"
#include "map.h"


void initMap()
{
	int i,j;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
			for (j = 0; j < MAP_WIDTH; j++)
			{
				map[i][j] = PRIORIY_EMPTY; // inicialmente nao tem nada no mapa
			}
	}
}

void draw(tContext sContext, const uint32_t oneChannel[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i, const uint8_t priority)
{
	int i,j;
	unsigned char map_prio;
	
	GrContextBackgroundSet(&sContext, ClrBlack);
	for (i = 0; i < height; i++)
	{
			for (j = 0; j < width; j++)
			{
					if( oneChannel[i*width + j] > 0x20)
					{
						map_prio = map[i+offset_i][offset_j + j];
						// as coordenadas i,j sao relativas à imagem e nao ao mapa por isso considera o offset(para ter coordenadas absolutas)
						if(priority > map_prio) // desenha apenas se tiver mais prioridade que o pixel atual
						{
							GrContextForegroundSet(&sContext, oneChannel[i*width + j]);
							GrPixelDraw(&sContext,offset_j + j,i+offset_i);
							map[i+offset_i][offset_j + j] = priority;
						}						
					}
					
			}
	}
}


void Ladder(tContext sContext)
{
	int i,j=0;
	 
	int numberOfAreas = 4;
	int numberOfLaddersInArea = 2;
	uint32_t oneChannel[LADDER_NUMBER_PIXELS];
	
	int ladderStarts[4][2] = {
	{10, 70},
	{60, 30},
	{0, 50},
	{100, 20}
	};
	
	for(i = 0; i < LADDER_NUMBER_PIXELS*3 - 3; i+=3)
	{
		oneChannel[j] = (ladder[i]<<16) + (ladder[i+1]<<8) + (ladder[i+2]);
		j++;
	}
	for(i=0; i< numberOfAreas;i++)
	{
		for(j=0; j< numberOfLaddersInArea;j++){
			draw(sContext, oneChannel, LADDER_HEIGHT, LADDER_WIDTH, ladderStarts[i][j], (127 - FLOOR_HEIGHT) - LADDER_HEIGHT*(i+1) - FLOOR_HEIGHT*i, PRIORIY_BACKGROUND);
		}
	}

}

void Sneaker(tContext sContext, uint16_t xOffset, uint8_t areaOffset)
{
	drawSneaker(sContext, xOffset, areaOffset, 0);
}
void Boss(tContext sContext, uint16_t xOffset, uint8_t areaOffset)
{
	drawSneaker(sContext, xOffset, areaOffset, 5);
}

void drawSneaker(tContext sContext, uint16_t xOffset, uint8_t areaOffset, uint8_t extraHeight)
{
	int i,j=0,sneakerHeight,headTopOffset;
	uint32_t oneChannelHead[HEAD_NUMBER_PIXELS],oneChannelLegs[LEGS_NUMBER_PIXELS],oneChannelLegsExtra[LEGS_EXTRA_NUMBER_PIXELS];
	
	for(i = 0; i < HEAD_NUMBER_PIXELS*3 - 3; i+=3)
	{
		oneChannelHead[j] = (head[i]<<16) + (head[i+1]<<8) + (head[i+2]);
		j++;
	}
	j=0;
	for(i = 0; i < LEGS_NUMBER_PIXELS*3 - 3; i+=3)
	{
		oneChannelLegs[j] = (legs[i]<<16) + (legs[i+1]<<8) + (legs[i+2]);
		j++;
	}
	j=0;
	for(i = 0; i < LEGS_EXTRA_NUMBER_PIXELS*3 - 3; i+=3)
	{
		oneChannelLegsExtra[j] = (legsExtra[i]<<16) + (legsExtra[i+1]<<8) + (legsExtra[i+2]);
		j++;
	}
	
	sneakerHeight = HEAD_HEIGHT + LEGS_HEIGHT + extraHeight;
	headTopOffset = (127 - FLOOR_HEIGHT) - sneakerHeight - (LADDER_HEIGHT + FLOOR_HEIGHT)*(areaOffset);
	
	draw(sContext, oneChannelHead, HEAD_HEIGHT, HEAD_WIDTH, xOffset, headTopOffset , PRIORITY_SNEAKER);
	for(i=0;i<extraHeight;i++)
	{		
		draw(sContext, oneChannelLegsExtra, LEGS_EXTRA_HEIGHT, LEGS_EXTRA_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + i, PRIORITY_SNEAKER);
	}
	draw(sContext, oneChannelLegs, LEGS_HEIGHT, LEGS_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + extraHeight, PRIORITY_SNEAKER);
}


void Eddie(tContext sContext, uint16_t xOffset, uint8_t areaOffset)
{
	int i,j=0,eddieTopOffset;
	uint32_t oneChannel[EDDIE_NUMBER_PIXELS];
	int initialXPosition = 0;
	
	for(i = 0; i < EDDIE_NUMBER_PIXELS*3 - 3; i+=3)
	{
		oneChannel[j] = (eddie[i]<<16) + (eddie[i+1]<<8) + (eddie[i+2]);
		j++;
	}
//	flipVert(oneChannel, EDDIE_HEIGHT, EDDIE_WIDTH);
	eddieTopOffset = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT)*(areaOffset);
	draw(sContext, oneChannel, EDDIE_HEIGHT, EDDIE_WIDTH, initialXPosition + xOffset, eddieTopOffset, PRIORIY_EDDIE);
}


void Item(tContext sContext, uint16_t xOffset, uint8_t areaOffset, uint8_t colorIndex)
{
	int i,j=0,itemTopOffset;
	uint32_t oneChannel[ITEM_NUMBER_PIXELS];
	uint32_t colors[] = {ClrMagenta,ClrPink,ClrYellow,ClrYellowGreen,ClrGreen};
	for(i = 0; i < ITEM_NUMBER_PIXELS*3 - 3; i+=3)
	{
		oneChannel[j] = (item[i]<<16) + (item[i+1]<<8) + (item[i+2]);
		if(oneChannel[j] != ClrBlack)
		{
			oneChannel[j] = colors[colorIndex];
		}
		j++;
	}
	itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT)*(areaOffset) + 1;
	draw(sContext, oneChannel, ITEM_HEIGHT, ITEM_WIDTH, xOffset, itemTopOffset, PRIORITY_ITEM);
}

void Score(tContext sContext)
{
	int i,j,lifes = 3;
	int lifeIndicatorLenght = 8;
	int start;
	
	GrStringDrawCentered(&sContext,"00000", -1, 64, (sContext.psFont->ui8Height)/2, true);	
	for(i=0;i<lifes;i++)
	{
		start = 48 + i*(lifeIndicatorLenght + 3);
		for(j=0;j<lifeIndicatorLenght;j++)
		{
			GrPixelDraw(&sContext,start + j,(sContext.psFont->ui8Height) + 2);
		}
	}
}

void Floor(tContext sContext)
{
	int i,j=0;
	uint32_t oneChannel[FLOOR_NUMBER_PIXELS];
	int numberOfFloors = 5;
	
	for(i = 0; i < FLOOR_NUMBER_PIXELS*3 - 3; i+=3)
	{
		oneChannel[j] = (floor_[i]<<16) + (floor_[i+1]<<8) + (floor_[i+2]);
		j++;
	}
	
	
	for(i = 0; i < numberOfFloors; i++)
	{
			draw(sContext, oneChannel, FLOOR_HEIGHT, FLOOR_WIDTH, 0, (127 - FLOOR_HEIGHT) - i*(FLOOR_HEIGHT + LADDER_HEIGHT), PRIORIY_BACKGROUND);
	}
}

