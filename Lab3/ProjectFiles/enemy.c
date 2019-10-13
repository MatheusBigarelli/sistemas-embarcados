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
void drawBoss(Image img)
{
	int a;
	palette[ENEMY_HEAD] = 0x00AA814E; // Altera a cor da cabeca para identificar como chefao
	if(clearBoss)
	{
		a = currentScore;
	}
	drawEnemy(img, 5 - currentScore/10);
}

void drawEnemy(Image img, uint8_t extraHeight)
{
	int i, j = 0, sneakerHeight, headTopOffset;
	
	sneakerHeight = HEAD_HEIGHT + LEGS_HEIGHT + extraHeight;
	headTopOffset = (127 - FLOOR_HEIGHT) - sneakerHeight - (LADDER_HEIGHT + FLOOR_HEIGHT) * (img.areaOffset);
	img.colorIndex = ENEMY_HEAD;
	img.data = head;
	img.width = HEAD_WIDTH;
	img.height = HEAD_HEIGHT;
	img.y = headTopOffset;
	if(clearBoss)
	{
		if(img.dirX == RIGHT)
		{
			img.x--;
			img.y--;
			clear(img);
			img.y++;
			img.x++;
		}
		if(img.dirX == LEFT)
		{
			img.x++;
			img.y--;
			clear(img);
			img.y++;
			img.x--;
		}
	}
	
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