#include "draw.h"

extern uint8_t eddie_hat[];
extern uint8_t eddie_body1[];
extern uint8_t eddie_body2[];
extern uint8_t eddie_body_moving[];
extern uint8_t eddie_body_moving2[];
extern uint8_t eddie_shirt[];

Direction currentDir;
Direction lastFacingDir = NONE;
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

		if(eddie.dirX != lastFacingDir && lastFacingDir != NONE)
		{		
			flipVert(eddie_hat, EDDIE_HAT_HEIGHT, EDDIE_HAT_WIDTH);
			flipVert(eddie_body1, EDDIE_BODY1_HEIGHT, EDDIE_BODY1_WIDTH);
			flipVert(eddie_shirt, EDDIE_SHIRT_HEIGHT, EDDIE_SHIRT_WIDTH);
			flipVert(eddie_body2, EDDIE_BODY2_HEIGHT, EDDIE_BODY2_WIDTH);
			flipVert(eddie_body_moving, EDDIE_BODY_MOVING_HEIGHT, EDDIE_BODY_MOVING_WIDTH);
			flipVert(eddie_body_moving2, EDDIE_BODY_MOVING2_HEIGHT, EDDIE_BODY_MOVING2_WIDTH);
		}	

		eddieTopOffset = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (eddie.areaOffset);
		if(eddie.dirY != LEFT) // Eddie nao esta caindo do pulo, pode imprimir na ordem normal
		{
			eddie.colorIndex = EDDIE_HAT;
			eddie.data = eddie_hat;
			eddie.width = EDDIE_HAT_WIDTH;
			eddie.height = EDDIE_HAT_HEIGHT;
			eddie.y += eddieTopOffset;
			draw(eddie); // Desenha cabecao
			eddie.colorIndex = EDDIE_BODY;
			eddie.data = eddie_body1;
			eddie.width = EDDIE_BODY1_WIDTH;
			eddie.height = EDDIE_BODY1_HEIGHT;
			eddie.y += EDDIE_HAT_HEIGHT;
			draw(eddie); // Desenha cabeca
			eddie.colorIndex = EDDIE_SHIRT;
			eddie.data = eddie_shirt;
			eddie.width = EDDIE_SHIRT_WIDTH;
			eddie.height = EDDIE_SHIRT_HEIGHT;
			eddie.y += EDDIE_BODY1_HEIGHT;
			draw(eddie); // Desenha camisa
			eddie.colorIndex = EDDIE_BODY;
			eddie.y += EDDIE_SHIRT_HEIGHT;
			eddie.data = eddie_body2;
			eddie.width = EDDIE_BODY2_WIDTH;
			eddie.height = EDDIE_BODY2_HEIGHT;
			clear(eddie);
			draw(eddie);
		}
		else // Eddie caindo, imprime na ordem contraria(comecando dos pes)
		{
			eddie.colorIndex = EDDIE_BODY;
			eddie.data = eddie_body2;
			eddie.width = EDDIE_BODY2_WIDTH;
			eddie.height = EDDIE_BODY2_HEIGHT;
			eddie.y += eddieTopOffset + EDDIE_HAT_HEIGHT + EDDIE_BODY1_HEIGHT + EDDIE_SHIRT_HEIGHT;
			draw(eddie); // Desenha os pes
			eddie.y -= EDDIE_SHIRT_HEIGHT;
			eddie.colorIndex = EDDIE_SHIRT;
			eddie.data = eddie_shirt;
			eddie.width = EDDIE_SHIRT_WIDTH;
			eddie.height = EDDIE_SHIRT_HEIGHT;
			draw(eddie); // Desenha a camisa
			eddie.y -= EDDIE_BODY1_HEIGHT;
			eddie.colorIndex = EDDIE_BODY;
			eddie.data = eddie_body1;
			eddie.width = EDDIE_BODY1_WIDTH;
			eddie.height = EDDIE_BODY1_HEIGHT;
			draw(eddie); // Desenha cabeca
			eddie.y -= EDDIE_HAT_HEIGHT;
			eddie.colorIndex = EDDIE_HAT;
			eddie.data = eddie_hat;
			eddie.width = EDDIE_HAT_WIDTH;
			eddie.height = EDDIE_HAT_HEIGHT;
			draw(eddie);
		}
		
		if(eddie.isMoving && eddie.dirY == NONE) // Eddie esta se movendo na horizontal(dirY = NONE)
		{
			if(eddieFeetUp)
			{				
				clear(eddie);
				eddie.data = eddie_body_moving;
				eddie.width = EDDIE_BODY_MOVING_WIDTH;
				eddie.height = EDDIE_BODY_MOVING_HEIGHT;
				draw(eddie);
			}
			else
			{				
				clear(eddie);
				eddie.data = eddie_body_moving2;
				eddie.width = EDDIE_BODY_MOVING2_WIDTH;
				eddie.height = EDDIE_BODY_MOVING2_HEIGHT;
				draw(eddie);
			}
			eddieFeetUp = !eddieFeetUp;
			
		}
		lastFacingDir = eddie.dirX;
		return;
	}
}