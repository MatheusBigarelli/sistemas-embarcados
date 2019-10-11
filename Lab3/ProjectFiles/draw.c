#include "draw.h"

extern bool eddieCollectedItem;
extern bool eddieCollidedWithEnemy;
extern uint8_t areaOfItemCollected;
extern Image ladderImage;

// Paleta de cores usada no jogo
uint32_t palette[9];

// buffer com 2 niveis de profundidade, guarda o frame atual e o anterior
unsigned char buffer[2][MAP_HEIGHT][MAP_WIDTH]; 

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

void checkColision(ColorIndex index1, ColorIndex index2, uint16_t areaOffset)
{
	// As unicas colisoes que importam sao as do Eddie com Item/Inimigo
	if(index1 == EDDIE_SHIRT || index1 == EDDIE_HAT || index1 == EDDIE_BODY) // index1 eh o Eddie
	{
		if(index2 == ITEM) // Eddie coletando item
		{
			areaOfItemCollected = areaOffset; // O Eddie so pode coletar o item que esta na mesma area que ele
			eddieCollectedItem = true;
		}
		if(index2 == ENEMY_HEAD || index2 == ENEMY_LEGS) // Eddie colidindo com inimigo
		{
			eddieCollidedWithEnemy = true;
		}		
		if(index2 == LADDER)
		{
			ladderImage.needsUpdate = true;
		}
	}
	else if(index1 == ENEMY_HEAD || index1 == ENEMY_LEGS)
	{
		if(index2 == LADDER)
		{
			ladderImage.needsUpdate = true;
		}
	}
	else if(index1 == ITEM )
	{
		if(index2 == LADDER)
		{
			ladderImage.needsUpdate = true;
		}
	}
}
void clear(Image img)
{
	int i,j;
	ColorIndex current;
	for (i = 0; i < img.height; i++)
	{
		for (j = 0; j < img.width; j++)
		{
			current = buffer[0][i + img.y][img.x + j];
			if (current == img.colorIndex)
			{
				current = buffer[1][i + img.y][img.x + j];
				GrContextForegroundSet(&sContext, palette[current]);
				GrPixelDraw(&sContext, img.x + j, i + img.y);
				buffer[0][i + img.y][img.x + j] = current;
			}			
		}
	}
}

void clearEddie(Image eddie)
{
	int i,j;
	ColorIndex colorOnPreviousFrame;
	int y = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (eddie.areaOffset);
	for (i = 0; i < EDDIE_HEIGHT; i++)
	{
		for (j = 0; j < EDDIE_WIDTH + 1; j++)
		{
			if (buffer[0][i + y][eddie.x + j-1] != EMPTY)
			{
				colorOnPreviousFrame = buffer[1][i + y][eddie.x -1 + j];
				GrContextForegroundSet(&sContext, palette[colorOnPreviousFrame]);
				GrPixelDraw(&sContext, eddie.x + j-1, i + y);
				buffer[0][i + y][eddie.x + j-1] = colorOnPreviousFrame;
			}
		}
	}
}

void draw(Image img)
{
	int i, j;
	ColorIndex newIndex;
	ColorIndex currentIndex;
	ColorIndex previousBufferPixel;		  // Usando quando o objeto esta movendo e seu rastro deve ser apagado.
	bool firstPixel = false;

	if(img.isMoving)
	{
		for (i = 0; i < img.height; i++)
		{
			for (j = 0; j < img.width; j++)
			{
				currentIndex = buffer[0][i + img.y ][img.x + j - img.dirX];
				if(currentIndex == img.colorIndex)
				{
					previousBufferPixel = buffer[1][i + img.y][img.x + j - img.dirX];
					GrContextForegroundSet(&sContext, palette[previousBufferPixel]);
					GrPixelDraw(&sContext, img.x + j - img.dirX, i + img.y);
					buffer[0][i + img.y][img.x + j - img.dirX] = previousBufferPixel;
				}
		
			}
		}
	}

	GrContextBackgroundSet(&sContext, ClrBlack);
	for (i = 0; i < img.height; i++)
	{
		for (j = 0; j < img.width; j++)
		{
			newIndex = img.data[i * img.width + j];
			// as coordenadas i,j sao relativas � imagem e nao ao mapa por isso considera o offset(para ter coordenadas absolutas)
			currentIndex = buffer[0][i + img.y][img.x + j];
			
			checkColision(newIndex, currentIndex, img.areaOffset);

			if (newIndex > currentIndex) // desenha apenas se tiver mais prioridade que o pixel atual
			{
				GrContextForegroundSet(&sContext, palette[newIndex]);
				GrPixelDraw(&sContext, img.x + j, i + img.y);				

				// Move o pixel atual para o "fundo" do buffer
				buffer[1][i + img.y][img.x + j] = currentIndex;
				// Atualiza o pixel atual com o novo valor
				buffer[0][i + img.y][img.x + j] = newIndex;
			}

		}
		firstPixel = false;
	}
}