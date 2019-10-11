#include "draw.h"


extern bool eddieCollectedItem;
extern bool eddieCollidedWithEnemy;
extern uint8_t areaOfItemCollected;


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
	}
	// Esse segundo if nao eh necessario, esse verificacao ocorreria quando o outro objeto (index2) estivesse sendo renderizado
	else if(index2 == EDDIE_SHIRT || index2 == EDDIE_HAT || index2 == EDDIE_BODY) // index2 eh o Eddie
	{
		if(index1 == ITEM) // Eddie coletando item
		{
			areaOfItemCollected = areaOffset;
			eddieCollectedItem = true;
		}
		if(index1 == ENEMY_HEAD || index1 == ENEMY_LEGS) // Eddie colidindo com inimigo
		{
			eddieCollidedWithEnemy = true;
		}
	}
}

void draw2(Image img)
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
				currentIndex = buffer[0][i + img.y + img.dirY][img.x + j - img.dirX];
				if(currentIndex == img.colorIndex)
				{
					previousBufferPixel = buffer[1][i + img.y + img.dirY][img.x + j - img.dirX];
					GrContextForegroundSet(&sContext, palette[previousBufferPixel]);
					GrPixelDraw(&sContext, img.x + j - img.dirX, i + img.y + img.dirY);
					buffer[0][i + img.y + img.dirY][img.x + j - img.dirX] = previousBufferPixel;
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

// void draw(const uint8_t image[], const uint16_t height, const uint16_t width, const uint16_t offset_j, const uint16_t offset_i, Direction dir, ColorIndex index)
// {
// 	int i, j;
// 	ColorIndex newPixel;
// 	ColorIndex currentPixel;
// 	ColorIndex previousBufferPixel;		  // Usando quando o objeto esta movendo e seu rastro deve ser apagado.
// 	bool firstPixel = false;
// 	TypeOfColision colision;
// 	if (dir == RIGHT) // Indo para direita
// 	{
// 		for (i = 0; i < height; i++)
// 		{
// 			for (j = 0; j < width; j++)
// 			{
// 				currentPixel = buffer[0][i + offset_i][offset_j + j - 1];
// 				if(currentPixel == index)
// 				{
// 					previousBufferPixel = buffer[1][i + offset_i][offset_j + j -1];
// 					GrContextForegroundSet(&sContext, palette[previousBufferPixel]);
// 					GrPixelDraw(&sContext, offset_j + j - 1, i + offset_i);
// 					buffer[0][i + offset_i][offset_j + j - 1] = previousBufferPixel;
// 				}
		
// 			}
// 		}
// 	}
// 	else if (dir == LEFT) // Indo para esquerda
// 	{
// 		for (i = 0; i < height; i++)
// 		{
// 			for (j = 0; j < width; j++)
// 			{
// 				currentPixel = buffer[0][i + offset_i][offset_j + j + 1];
// 				if(currentPixel == index)
// 				{
// 					previousBufferPixel = buffer[1][i + offset_i][offset_j + j + 1];
// 					GrContextForegroundSet(&sContext, palette[previousBufferPixel]);
// 					GrPixelDraw(&sContext, offset_j + j + 1, i + offset_i);
// 					buffer[0][i + offset_i][offset_j + j + 1] = previousBufferPixel;
// 				}
				
// 			}
// 		}
// 	}
// 	else if (dir == UPDATE)
// 	{
// 		for (i = 0; i < height; i++)
// 		{
// 			for (j = 0; j < width; j++)
// 			{
// 				currentPixel = buffer[0][i + offset_i][offset_j + j ];
// 				if(currentPixel == index)
// 				{
// 					previousBufferPixel = buffer[1][i + offset_i][offset_j + j];
// 					GrContextForegroundSet(&sContext, palette[previousBufferPixel]);
// 					GrPixelDraw(&sContext, offset_j + j, i + offset_i);
// 					buffer[0][i + offset_i][offset_j + j ] = previousBufferPixel;
// 				}
				
// 			}
// 		}
// 	}
	
// 	GrContextBackgroundSet(&sContext, ClrBlack);
// 	for (i = 0; i < height; i++)
// 	{
// 		for (j = 0; j < width; j++)
// 		{
// 			newPixel = image[i * width + j];
// 			// as coordenadas i,j sao relativas � imagem e nao ao mapa por isso considera o offset(para ter coordenadas absolutas)
// 			currentPixel = buffer[0][i + offset_i][offset_j + j];
			
// 			colision = checkColision(newPixel, currentPixel);
// 			if(colision == EDDIE_ITEM)
// 			{
// 				areaOfItemCollected = (i + offset_i - (127 - FLOOR_HEIGHT))/(-1*(LADDER_HEIGHT + FLOOR_HEIGHT));
// 				eddieCollectedItem = true;		
				
// 			}
// 			else if(colision == EDDIE_ENEMY)
// 			{
// 				eddieCollidedWithEnemy = true;
// 			}
// 			if (newPixel > currentPixel) // desenha apenas se tiver mais prioridade que o pixel atual
// 			{
// 				GrContextForegroundSet(&sContext, palette[newPixel]);
// 				GrPixelDraw(&sContext, offset_j + j, i + offset_i);
				

// 				// Move o pixel atual para o "fundo" do buffer
// 				buffer[1][i + offset_i][offset_j + j] = currentPixel;
// 				// Atualiza o pixel atual com o novo valor
// 				buffer[0][i + offset_i][offset_j + j] = newPixel;
// 			}

// 		}
// 		firstPixel = false;
// 	}
// }