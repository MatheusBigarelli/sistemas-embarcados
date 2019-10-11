#include "draw.h"

extern uint8_t eddie_hat[];
extern uint8_t eddie_body1[];
extern uint8_t eddie_body2[];
extern uint8_t eddie_body_moving[];
extern uint8_t eddie_body_moving2[];
extern uint8_t eddie_shirt[];

Direction currentDir;
Direction lastFacingDir = RIGHT;
bool eddieFeetUp = false;

void drawEddie(Image eddie)
{
	int i, j = 0, eddieTopOffset;
	
	if(eddie.needsUpdate == false)
	{
		return; // Nao precisa fazer nada com o eddie.
	}
	else
	{

		if(eddie.dirX != lastFacingDir)
		{		
			flipVert(eddie_hat, EDDIE_HAT_HEIGHT, EDDIE_HAT_WIDTH);
			flipVert(eddie_body1, EDDIE_BODY1_HEIGHT, EDDIE_BODY1_WIDTH);
			flipVert(eddie_shirt, EDDIE_SHIRT_HEIGHT, EDDIE_SHIRT_WIDTH);
			flipVert(eddie_body2, EDDIE_BODY2_HEIGHT, EDDIE_BODY2_WIDTH);
			flipVert(eddie_body_moving, EDDIE_BODY_MOVING_HEIGHT, EDDIE_BODY_MOVING_WIDTH);
			flipVert(eddie_body_moving2, EDDIE_BODY_MOVING2_HEIGHT, EDDIE_BODY_MOVING2_WIDTH);
		}	

		eddieTopOffset = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (eddie.areaOffset);

		eddie.colorIndex = EDDIE_HAT;
		eddie.data = eddie_hat;
		eddie.width = EDDIE_HAT_WIDTH;
		eddie.height = EDDIE_HAT_HEIGHT;
		eddie.y = eddieTopOffset;
		draw(eddie);
		eddie.colorIndex = EDDIE_BODY;
		eddie.data = eddie_body1;
		eddie.width = EDDIE_BODY1_WIDTH;
		eddie.height = EDDIE_BODY1_HEIGHT;
		eddie.y += EDDIE_HAT_HEIGHT;
		draw(eddie);
		eddie.colorIndex = EDDIE_SHIRT;
		eddie.data = eddie_shirt;
		eddie.width = EDDIE_SHIRT_WIDTH;
		eddie.height = EDDIE_SHIRT_HEIGHT;
		eddie.y += EDDIE_BODY1_HEIGHT;
		draw(eddie);
		eddie.colorIndex = EDDIE_BODY;
		eddie.y += EDDIE_SHIRT_HEIGHT;
		if(eddie.isMoving)
		{
			if(eddieFeetUp)
			{
				eddie.data = eddie_body_moving;
				eddie.width = EDDIE_BODY_MOVING_WIDTH;
				eddie.height = EDDIE_BODY_MOVING_HEIGHT;
				draw(eddie);
			}
			else
			{
				eddie.data = eddie_body_moving2;
				eddie.width = EDDIE_BODY_MOVING2_WIDTH;
				eddie.height = EDDIE_BODY_MOVING2_HEIGHT;
				draw(eddie);
			}
			eddieFeetUp = !eddieFeetUp;
		}
		else
		{
			eddie.data = eddie_body2;
			eddie.width = EDDIE_BODY2_WIDTH;
			eddie.height = EDDIE_BODY2_HEIGHT;
			clear(eddie);
			draw(eddie);
		}
		lastFacingDir = eddie.dirX;
		return;
	}
}