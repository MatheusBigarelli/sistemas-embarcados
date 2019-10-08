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
					if( oneChannel[i*width + j] != ClrBlack)
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
	uint32_t oneChannel[204];
	
	int ladderStarts[4][2] = {
	{10, 70},
	{60, 30},
	{0, 50},
	{100, 20}
	};
	
	for(i = 0; i < 204*3 - 3; i+=3)
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
	int i,j=0;
	uint32_t oneChannel[60];
	
	for(i = 0; i < 60*3 - 3; i+=3)
	{
		oneChannel[j] = (sneaker[i]<<16) + (sneaker[i+1]<<8) + (sneaker[i+2]);
		j++;
	}
	draw(sContext, oneChannel, SNEAKER_HEIGHT, SNEAKER_WIDTH, xOffset, (127 - FLOOR_HEIGHT) - SNEAKER_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT)*(areaOffset), PRIORITY_SNEAKER);
}

void Eddie(tContext sContext, uint16_t xOffset, uint8_t areaOffset)
{
	int i,j=0;
	uint32_t oneChannel[90];
	int initialXPosition = 0;
	
	for(i = 0; i < 90*3 - 3; i+=3)
	{
		oneChannel[j] = (eddie[i]<<16) + (eddie[i+1]<<8) + (eddie[i+2]);
		j++;
	}
	if(xOffset > 0)
	{
		xOffset = 1;
	}
	else if(xOffset < 0)
	{
		xOffset = -1;
	}
	xOffset = 20;
	draw(sContext, oneChannel, EDDIE_HEIGHT, EDDIE_WIDTH, initialXPosition + xOffset,(127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT)*(areaOffset), PRIORIY_EDDIE);
}

void Floor(tContext sContext)
{
	int i,j=0;
	uint32_t oneChannel[384];
	int numberOfFloors = 5;
	
	for(i = 0; i < 384*3 - 3; i+=3)
	{
		oneChannel[j] = (floor_[i]<<16) + (floor_[i+1]<<8) + (floor_[i+2]);
		j++;
	}
	
	
	for(i = 0; i < numberOfFloors; i++)
	{
			draw(sContext, oneChannel, FLOOR_HEIGHT, FLOOR_WIDTH, 0, (127 - FLOOR_HEIGHT) - i*(FLOOR_HEIGHT + LADDER_HEIGHT), PRIORIY_BACKGROUND);
	}
}
