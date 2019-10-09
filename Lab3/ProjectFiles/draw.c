#include "draw.h"

#include "floor.h"
#include "sneaker.h"
#include "name.h"
#include "item.h"
#include "eddie.h"
#include "ladder.h"
#include "map.h"

extern tContext sContext;
uint32_t ladderColor;

void initMap(void)
{
	int i,j;
	for (i = 0; i < MAP_HEIGHT; i++)
	{
			for (j = 0; j < MAP_WIDTH; j++)
			{
				map[i][j] = PRIORITY_EMPTY; // inicialmente nao tem nada no mapa
			}
	}
}

void draw(const uint32_t oneChannel[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i, const uint8_t priority, const uint8_t delta)
{
	int i,j;
	bool clearedPixel = false;
	unsigned char mapPrio;
	
	GrContextBackgroundSet(&sContext, ClrBlack);
	for (i = 0; i < height; i++)
	{
			for (j = 0; j < width; j++)
			{
					if( oneChannel[i*width + j] > 0x20)
					{
						mapPrio = map[i+offset_i][offset_j + j];
						// as coordenadas i,j sao relativas à imagem e nao ao mapa por isso considera o offset(para ter coordenadas absolutas)
						if(priority > mapPrio) // desenha apenas se tiver mais prioridade que o pixel atual
						{
							GrContextForegroundSet(&sContext, oneChannel[i*width + j]);
							GrPixelDraw(&sContext,offset_j + j,i+offset_i);
							if(delta == 1 && !clearedPixel) // movendo o objeto para direita 
							{								
								GrContextForegroundSet(&sContext, ClrBlack);
								GrPixelDraw(&sContext,offset_j + j - 1,i+offset_i); // apaga o pixel antigo
								clearedPixel = true;
							}
							
							map[i+offset_i][offset_j + j] = priority;
						}						
					}
					
			}
			clearedPixel = false; // Movendo para nova linha, agora um novo pixel precisa ser apagado
	}
}



void drawEddie(uint16_t xOffset, uint8_t areaOffset)
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
	draw(oneChannel, EDDIE_HEIGHT, EDDIE_WIDTH, initialXPosition + xOffset, eddieTopOffset, PRIORITY_EDDIE,0);
}


void drawSneaker(uint16_t xOffset, uint8_t areaOffset)
{
	drawEnemy(xOffset, areaOffset, 0);
}
void drawBoss(uint16_t xOffset, uint8_t areaOffset)
{
	drawEnemy(xOffset, areaOffset, 5);
}

void drawEnemy(uint16_t xOffset, uint8_t areaOffset, uint8_t extraHeight)
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
	
	draw(oneChannelHead, HEAD_HEIGHT, HEAD_WIDTH, xOffset, headTopOffset , PRIORITY_SNEAKER,0);
	for(i=0;i<extraHeight;i++)
	{		
		draw(oneChannelLegsExtra, LEGS_EXTRA_HEIGHT, LEGS_EXTRA_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + i, PRIORITY_SNEAKER,0);
	}
	draw(oneChannelLegs, LEGS_HEIGHT, LEGS_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + extraHeight, PRIORITY_SNEAKER,0);
}






void drawItem(uint16_t xOffset, uint8_t areaOffset, uint8_t colorIndex, uint8_t delta)
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
	draw(oneChannel, ITEM_HEIGHT, ITEM_WIDTH, xOffset, itemTopOffset, PRIORITY_ITEM,delta);
}

void drawLadder(void)
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
		if(oneChannel[j] > 0x10)
		{
			ladderColor = oneChannel[j];
		}
		j++;
	}
	for(i=0; i< numberOfAreas;i++)
	{
		for(j=0; j< numberOfLaddersInArea;j++){
			draw(oneChannel, LADDER_HEIGHT, LADDER_WIDTH, ladderStarts[i][j], (127 - FLOOR_HEIGHT) - LADDER_HEIGHT*(i+1) - FLOOR_HEIGHT*i, PRIORITY_BACKGROUND,0);
		}
	}

}

void drawScore(void)
{
	int i,j,lifes = 3;
	int lifeIndicatorLenght = 8;
	int start;
	GrContextForegroundSet(&sContext, ClrWhite);
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

void drawFloor(void)
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
			draw(oneChannel, FLOOR_HEIGHT, FLOOR_WIDTH, 0, (127 - FLOOR_HEIGHT) - i*(FLOOR_HEIGHT + LADDER_HEIGHT), PRIORITY_BACKGROUND,0);
	}
}

