#include "draw.h"

extern uint8_t head[] ;
extern uint8_t legs[];
extern uint8_t legsExtra[];

extern uint32_t palette[9];

extern uint16_t currentScore;

void drawSneaker(Image img)
{
	palette[ENEMY_HEAD] = 0x006D8BBD;
	drawEnemy(img, 0);
}
bool clearBoss = false;

void clearOldBoss(Image oldBoss, uint8_t extraHeight)
{
	int i, j = 0, sneakerHeight, headTopOffset;
	
	sneakerHeight = HEAD_HEIGHT + LEGS_HEIGHT + extraHeight + 1; // O boss antigo tem 1 de altura a mais
	headTopOffset = (127 - FLOOR_HEIGHT) - sneakerHeight - (LADDER_HEIGHT + FLOOR_HEIGHT) * (oldBoss.areaOffset);
	oldBoss.colorIndex = ENEMY_HEAD;
	oldBoss.data = head;
	oldBoss.width = HEAD_WIDTH;
	oldBoss.height = HEAD_HEIGHT;
	oldBoss.y = headTopOffset;
	if(oldBoss.dirX == RIGHT)
	{
		oldBoss.x--;
		clear(oldBoss);
	}
	if(oldBoss.dirX == LEFT)
	{
		oldBoss.x++;
		clear(oldBoss);
	}
	oldBoss.colorIndex = ENEMY_LEGS;
	oldBoss.data = legsExtra;
	oldBoss.width = LEGS_EXTRA_WIDTH;
	oldBoss.height = LEGS_EXTRA_HEIGHT;
	for (i = 0; i < extraHeight; i++)
	{
		oldBoss.y = headTopOffset + HEAD_HEIGHT + i;		
		clear(oldBoss);
	}	
	if(extraHeight == 0)
	{
		oldBoss.colorIndex = ENEMY_LEGS;
		oldBoss.data = legs;
		oldBoss.width = LEGS_WIDTH;
		oldBoss.height = LEGS_HEIGHT;
		oldBoss.y = headTopOffset + HEAD_HEIGHT + extraHeight;
		clear(oldBoss);
	}
}

void drawBoss(Image img)
{
	int a;
	palette[ENEMY_HEAD] = 0x00AA814E; // Altera a cor da cabeca para identificar como chefao
	if(clearBoss)
	{
		clearOldBoss(img,5 - currentScore/10);
	}
	drawEnemy(img, 5 - currentScore/10);
}

void drawEnemy(Image img, uint8_t extraHeight)
{
	int i, j = 0, sneakerHeight, headTopOffset;
	
	sneakerHeight = HEAD_HEIGHT + LEGS_HEIGHT + extraHeight;
	headTopOffset = (127 - FLOOR_HEIGHT) - sneakerHeight - (LADDER_HEIGHT + FLOOR_HEIGHT) * (img.areaOffset) ;
	img.colorIndex = ENEMY_HEAD;
	img.data = head;
	img.width = HEAD_WIDTH;
	img.height = HEAD_HEIGHT;
	img.y = headTopOffset;
	draw(img);
	img.colorIndex = ENEMY_LEGS;
	img.data = legsExtra;
	img.width = LEGS_EXTRA_WIDTH;
	img.height = LEGS_EXTRA_HEIGHT;
	for (i = 0; i < extraHeight; i++)
	{
		img.y = headTopOffset + HEAD_HEIGHT + i;
		
		draw(img);
	}	
	flipVert(legs, LEGS_HEIGHT, LEGS_WIDTH);
	img.colorIndex = ENEMY_LEGS;
	img.data = legs;
	img.width = LEGS_WIDTH;
	img.height = LEGS_HEIGHT;
	img.y = headTopOffset + HEAD_HEIGHT + extraHeight;
	draw(img);
}