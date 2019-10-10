#include "collision.h"

bool objectCollidedWith(Object obj1, Object obj2)
{
	Object aux;
	uint8_t i;
	
	for (i = 0; i < 2; i++)
	{
		if (obj1.x <= obj2.x && obj2.x <= obj1.x+obj1.width)
		{
			if (obj1.y <= obj2.y && obj2.y <= obj1.y+obj1.height)
				return true;
			if (obj1.y <= obj2.y+obj2.height && obj2.y+obj2.height <= obj1.y+obj1.height)
				return true;
		}

		if (obj1.x <= obj2.x+obj2.width && obj2.x+obj2.width <= obj1.x+obj1.width)
		{
			if (obj1.y <= obj2.y && obj2.y <= obj1.y+obj1.height)
				return true;
			if (obj1.y <= obj2.y+obj2.height && obj2.y+obj2.height <= obj1.y+obj1.height)
				return true;
		}
		aux = obj1;
		obj1 = obj2;
		obj2 = aux;
	}
		
	return false;
}
