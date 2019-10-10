#include "draw.h"
#include "map.h"
#include "images.h"

extern tContext sContext;
// Paleta de cores usada no jogo
uint32_t palette[9];

void initMap(void)
{
	int i, j;
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
			buffer[0][i][j] = EMPTY; // inicialmente nao tem nada no buffer
			buffer[1][i][j] = EMPTY; // inicialmente nao tem nada no buffer
		}
	}
}

void draw(const uint8_t image[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i, Direction dir, ColorIndex index)
{
	int i, j;
	uint8_t newPixel;
	uint8_t currentPixel;
	uint8_t previousBufferPixel;		  // Usando quando o objeto esta movendo e seu rastro deve ser apagado.
	bool firstPixel = false;

	if (dir == RIGHT) // Indo para direita
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				currentPixel = buffer[0][i + offset_i][offset_j + j - 1];
				if(currentPixel == index)
				{
					previousBufferPixel = buffer[1][i + offset_i][offset_j + j -1];
					GrContextForegroundSet(&sContext, palette[previousBufferPixel]);
					GrPixelDraw(&sContext, offset_j + j - 1, i + offset_i);
					buffer[0][i + offset_i][offset_j + j - 1] = previousBufferPixel;
				}
		
			}
		}
	}
	else if (dir == LEFT) // Indo para esquerda
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				currentPixel = buffer[0][i + offset_i][offset_j + j + 1];
				if(currentPixel == index)
				{
					previousBufferPixel = buffer[1][i + offset_i][offset_j + j + 1];
					GrContextForegroundSet(&sContext, palette[previousBufferPixel]);
					GrPixelDraw(&sContext, offset_j + j + 1, i + offset_i);
					buffer[0][i + offset_i][offset_j + j + 1] = previousBufferPixel;
				}
				
			}
		}
	}
	GrContextBackgroundSet(&sContext, ClrBlack);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			newPixel = image[i * width + j];
			currentPixel = buffer[0][i + offset_i][offset_j + j];
			// as coordenadas i,j sao relativas � imagem e nao ao mapa por isso considera o offset(para ter coordenadas absolutas)
			
			if (newPixel > currentPixel) // desenha apenas se tiver mais prioridade que o pixel atual
			{
				GrContextForegroundSet(&sContext, palette[newPixel]);
				GrPixelDraw(&sContext, offset_j + j, i + offset_i);
				

				// Move o pixel atual para o "fundo" do buffer
				buffer[1][i + offset_i][offset_j + j] = currentPixel;
				// Atualiza o pixel atual com o novo valor
				buffer[0][i + offset_i][offset_j + j] = newPixel;
			}

		}
		firstPixel = false;
	}
}

void drawEddie(uint16_t xOffset, uint8_t areaOffset)
{
	int i, j = 0, eddieTopOffset;
	int initialXPosition = 0;
	//	flipVert(eddie, EDDIE_HEIGHT, EDDIE_WIDTH);
	
	eddieTopOffset = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (areaOffset);
	draw(eddie_hat, EDDIE_HAT_HEIGHT, EDDIE_HAT_WIDTH, initialXPosition + xOffset, eddieTopOffset,NONE,EDDIE_HAT);
	eddieTopOffset += EDDIE_HAT_HEIGHT;
	draw(eddie_body1, EDDIE_BODY1_HEIGHT, EDDIE_BODY1_WIDTH, initialXPosition + xOffset, eddieTopOffset,NONE,EDDIE_HAT);
	eddieTopOffset += EDDIE_BODY1_HEIGHT;
	draw(eddie_shirt, EDDIE_SHIRT_HEIGHT, EDDIE_SHIRT_WIDTH, initialXPosition + xOffset, eddieTopOffset,NONE,EDDIE_HAT);
	eddieTopOffset += EDDIE_SHIRT_HEIGHT;
	draw(eddie_body2, EDDIE_BODY2_HEIGHT, EDDIE_BODY2_WIDTH, initialXPosition + xOffset, eddieTopOffset,NONE,EDDIE_HAT);
	
}

void drawSneaker(uint16_t xOffset, uint8_t areaOffset, Direction dir)
{
	drawEnemy(xOffset, areaOffset, 0, dir);
}
void drawBoss(uint16_t xOffset, uint8_t areaOffset, Direction dir)
{
	drawEnemy(xOffset, areaOffset, 5, dir);
}

void drawEnemy(uint16_t xOffset, uint8_t areaOffset, uint8_t extraHeight, Direction dir)
{
	int i, j = 0, sneakerHeight, headTopOffset;

	sneakerHeight = HEAD_HEIGHT + LEGS_HEIGHT + extraHeight;
	headTopOffset = (127 - FLOOR_HEIGHT) - sneakerHeight - (LADDER_HEIGHT + FLOOR_HEIGHT) * (areaOffset);
	draw(head, HEAD_HEIGHT, HEAD_WIDTH, xOffset, headTopOffset,dir,ENEMY_HEAD);
	for (i = 0; i < extraHeight; i++)
	{
		draw(legsExtra, LEGS_EXTRA_HEIGHT, LEGS_EXTRA_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + i,dir,ENEMY_LEGS);
	}
	draw(legs, LEGS_HEIGHT, LEGS_WIDTH, xOffset, headTopOffset + HEAD_HEIGHT + extraHeight,dir,ENEMY_LEGS);
}

void drawItem(uint16_t xOffset, uint8_t areaOffset, Direction dir)
{
	int i, j = 0, itemTopOffset;
	itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT) * (areaOffset) + 1;
	draw(item, ITEM_HEIGHT, ITEM_WIDTH, xOffset, itemTopOffset,dir,ITEM);
}

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
			draw(ladder, LADDER_HEIGHT, LADDER_WIDTH, ladderStarts[i][j], (127 - FLOOR_HEIGHT) - LADDER_HEIGHT * (i + 1) - FLOOR_HEIGHT * i,NONE,LADDER);
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
		draw(floor_, FLOOR_HEIGHT, FLOOR_WIDTH, 0, (127 - FLOOR_HEIGHT) - i * (FLOOR_HEIGHT + LADDER_HEIGHT),NONE,FLOOR);
	}
}
