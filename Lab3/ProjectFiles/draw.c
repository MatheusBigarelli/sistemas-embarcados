#include "draw.h"
#include "map.h"
#include "images.h"

extern tContext sContext;
// Paleta de cores usada no jogo
uint32_t palette[9]; 

void initMap(void)
{
	int i,j;
	palette[EMPTY] = ClrBlack;
	palette[LADDER] = 0x00EC9852;
	palette[FLOOR] = 0x0048749F;
	palette[ITEM] = 0x00C97ABB;
	palette[ENEMY_LEGS] = ClrWhite;
	palette[ENEMY_HEAD] = 0x006D8BBD;
	palette[EDDIE_SHIRT] = 0x0049A042;
	palette[EDDIE_HAT] = 0x0049919F;
	palette[EDDIE_BODY] = 0x00DAE857;
	
	
	for (i = 0; i < MAP_HEIGHT; i++)
	{
			for (j = 0; j < MAP_WIDTH; j++)
			{
				map[i][j] = EMPTY; // inicialmente nao tem nada no mapa
			}
	}
}

void draw(const uint8_t image[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i)
{
	int i,j;
	uint8_t index;
	unsigned char mapPrio;
	
	GrContextBackgroundSet(&sContext, ClrBlack);
	for (i = 0; i < height; i++)
	{
			for (j = 0; j < width; j++)
			{
				index = image[i*width + j];
					if( index != EMPTY)
					{
						mapPrio = map[i+offset_i][offset_j + j];
						// as coordenadas i,j sao relativas � imagem e nao ao mapa por isso considera o offset(para ter coordenadas absolutas)
						if(index > mapPrio) // desenha apenas se tiver mais prioridade que o pixel atual
						{
							GrContextForegroundSet(&sContext, palette[index]);
							GrPixelDraw(&sContext,offset_j + j,i+offset_i);
							
							map[i+offset_i][offset_j + j] = index;
						}						
					}
					
			}
	}
}



void drawEddie(uint16_t xOffset, uint8_t areaOffset)
{
	int i,j=0,eddieTopOffset;
	int initialXPosition = 0;
//	flipVert(eddie, EDDIE_HEIGHT, EDDIE_WIDTH);
	eddieTopOffset = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT)*(areaOffset);
	draw(eddie, EDDIE_HEIGHT, EDDIE_WIDTH, initialXPosition + xOffset, eddieTopOffset);
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
	
	sneakerHeight = HEAD_HEIGHT + LEGS_HEIGHT + extraHeight;
	headTopOffset = (127 - FLOOR_HEIGHT) - sneakerHeight - (LADDER_HEIGHT + FLOOR_HEIGHT)*(areaOffset);
	
	draw(head, HEAD_HEIGHT, HEAD_WIDTH, xOffset, headTopOffset);
	for(i=0;i<extraHeight;i++)
	{		
		draw(legsExtra, LEGS_EXTRA_HEIGHT, LEGS_EXTRA_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + i);
	}
	draw(legs, LEGS_HEIGHT, LEGS_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + extraHeight);
}


void drawItem(uint16_t xOffset, uint8_t areaOffset)
{
	int i,j=0,itemTopOffset;
	itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT)*(areaOffset) + 1;
	draw(item, ITEM_HEIGHT, ITEM_WIDTH, xOffset, itemTopOffset);
}

void drawLadder(void)
{
	int i,j=0;
	 
	int numberOfAreas = 4;
	int numberOfLaddersInArea = 2;
	
	int ladderStarts[4][2] = {
	{10, 70},
	{60, 30},
	{0, 50},
	{100, 20}
	};	

	for(i=0; i< numberOfAreas;i++)
	{
		for(j=0; j< numberOfLaddersInArea;j++){
			draw(ladder, LADDER_HEIGHT, LADDER_WIDTH, ladderStarts[i][j], (127 - FLOOR_HEIGHT) - LADDER_HEIGHT*(i+1) - FLOOR_HEIGHT*i);
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
	int numberOfFloors = 5;
	for(i = 0; i < numberOfFloors; i++)
	{
			draw(floor_, FLOOR_HEIGHT, FLOOR_WIDTH, 0, (127 - FLOOR_HEIGHT) - i*(FLOOR_HEIGHT + LADDER_HEIGHT));
	}
}

