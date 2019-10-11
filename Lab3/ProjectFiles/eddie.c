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
		eddieTopOffset = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (eddie.areaOffset);

		eddie.colorIndex = EDDIE_HAT;
		eddie.data = eddie_hat;
		eddie.width = EDDIE_HAT_WIDTH;
		eddie.height = EDDIE_HAT_HEIGHT;
		eddie.yOffset = eddieTopOffset;
		draw2(eddie);
		eddie.colorIndex = EDDIE_BODY;
		eddie.data = eddie_body1;
		eddie.width = EDDIE_BODY1_WIDTH;
		eddie.height = EDDIE_BODY1_HEIGHT;
		eddie.yOffset += EDDIE_HAT_HEIGHT;
		draw2(eddie);
		eddie.colorIndex = EDDIE_SHIRT;
		eddie.data = eddie_shirt;
		eddie.width = EDDIE_SHIRT_WIDTH;
		eddie.height = EDDIE_SHIRT_HEIGHT;
		eddie.yOffset += EDDIE_BODY1_HEIGHT;
		draw2(eddie);
		eddie.colorIndex = EDDIE_BODY;
		eddie.data = eddie_body2;
		eddie.width = EDDIE_BODY2_WIDTH;
		eddie.height = EDDIE_BODY2_HEIGHT;
		eddie.yOffset += EDDIE_SHIRT_HEIGHT;
		draw2(eddie);
		// currentDir = UPDATE;
		return;
	}
	
	// if(eddie.dir != lastFacingDir)
	// {		
	// 	flipVert(eddie_hat, EDDIE_HAT_HEIGHT, EDDIE_HAT_WIDTH);
	// 	flipVert(eddie_body1, EDDIE_BODY1_HEIGHT, EDDIE_BODY1_WIDTH);
	// 	flipVert(eddie_shirt, EDDIE_SHIRT_HEIGHT, EDDIE_SHIRT_WIDTH);
	// 	flipVert(eddie_body2, EDDIE_BODY2_HEIGHT, EDDIE_BODY2_WIDTH);
	// 	flipVert(eddie_body_moving, EDDIE_BODY_MOVING_HEIGHT, EDDIE_BODY_MOVING_WIDTH);
	// 	flipVert(eddie_body_moving2, EDDIE_BODY_MOVING2_HEIGHT, EDDIE_BODY_MOVING2_WIDTH);
	// }		
	
	// // Eddie esta se movendo para direcao dir
	// draw(eddie_hat, EDDIE_HAT_HEIGHT, EDDIE_HAT_WIDTH, initialXPosition + xOffset, eddieTopOffset,dir,EDDIE_HAT);
	// eddieTopOffset += EDDIE_HAT_HEIGHT;
	// draw(eddie_body1, EDDIE_BODY1_HEIGHT, EDDIE_BODY1_WIDTH, initialXPosition + xOffset, eddieTopOffset,dir,EDDIE_BODY);
	// eddieTopOffset += EDDIE_BODY1_HEIGHT;
	// draw(eddie_shirt, EDDIE_SHIRT_HEIGHT, EDDIE_SHIRT_WIDTH, initialXPosition + xOffset, eddieTopOffset,dir,EDDIE_SHIRT);
	// eddieTopOffset += EDDIE_SHIRT_HEIGHT;
	
	// // Animacao de movimento do eddie
	// if (eddieFeetUp)
	// {
	// 	draw(eddie_body_moving, EDDIE_BODY_MOVING_HEIGHT, EDDIE_BODY_MOVING_WIDTH, initialXPosition + xOffset, eddieTopOffset,dir,EDDIE_BODY);
	// 	eddieFeetUp = false;
	// }
	// else
	// {
	// 	draw(eddie_body_moving2, EDDIE_BODY_MOVING2_HEIGHT, EDDIE_BODY_MOVING2_WIDTH, initialXPosition + xOffset, eddieTopOffset,dir,EDDIE_BODY);
	// 	eddieFeetUp = true;
	// }	
	// currentDir = dir;
	// lastFacingDir = dir;
}