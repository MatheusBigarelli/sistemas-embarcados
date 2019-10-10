#include "collision.h"

int16_t widths[] = {EDDIE_WIDTH, ENEMY_WIDTH, ITEM_WIDTH, LADDER_WIDTH};
int16_t heights[] = {EDDIE_HEIGHT, ENEMY_HEIGHT, ITEM_HEIGHT, LADDER_HEIGHT};

bool collided(Character id_character_1, Character id_character_2)
{
	uint8_t width1, height1;
	uint8_t width2, height2;
	width1 = widths[id_character_1];
	width2 = widths[id_character_2];
	height1 = heights[id_character_1];
	height2 = heights[id_character_2];
	
	if (width1 > 1)
		return true;
	
	return false;
}




bool objectCollidedWith(Object obj1, Object obj2)
{
	if (obj1.x < obj2.x && obj1.x+obj1.width >= obj2.x)
	{
		if (obj1.y < obj2.y && obj1.y+obj1.height >= obj2.y)
			return true;
		if (obj1.y >= obj2.y && obj1.y+obj1.height <= obj2.y+obj2.height)
			return true;
		if (obj1.y > obj2.y && obj1.y+obj1.height >= obj2.y+obj2.height)
			return true;
	}
	if (obj1.x <= obj2.x+obj2.width && obj1.x+obj1.width >= obj2.x+obj2.width)
	{
		if (obj1.y < obj2.y && obj1.y+obj1.height >= obj2.y)
			return true;
		if (obj1.y >= obj2.y && obj1.y+obj1.height <= obj2.y+obj2.height)
			return true;
		if (obj1.y > obj2.y && obj1.y+obj1.height >= obj2.y+obj2.height)
			return true;
	}
	
	return false;
}
