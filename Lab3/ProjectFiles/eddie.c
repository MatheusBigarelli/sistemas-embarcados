#include "draw.h"
#include "stdlib.h"

extern uint8_t eddie_hat[];
extern uint8_t eddie_body1[];
extern uint8_t eddie_body2[];
extern uint8_t eddie_body_moving[];
extern uint8_t eddie_body_moving2[];
extern uint8_t eddie_shirt[];
extern uint8_t eddie_shirt_jumping[];

extern osMutexId mid_displayMutex;
extern bool joyMoving;
extern bool stopedMoving;

Direction currentDir;
Direction lastFacingDir = NONE;
bool eddieFeetUp = false;

int jumpHeight = 0;
extern Direction joyDir;
int longJump = 0;
Direction longJumpDir = NONE;

Direction eddieCanGoToLadder(uint16_t eddieXPosition, uint8_t eddieAreaOffset)
{
	int i,j;
	int ladderXPosition;
	int ladderStarts[NUMBER_OF_AREAS][NUMBER_OF_LADDERS_IN_AREA] = {
		{10, 70},
		{60, 30},
		{0, 50},
		{100, 20}};
	for (i = 0; i < NUMBER_OF_AREAS; i++)
	{
		for (j = 0; j < NUMBER_OF_LADDERS_IN_AREA; j++)
		{
			ladderXPosition = ladderStarts[i][j];
			if (eddieXPosition > ladderXPosition + 2 && eddieXPosition < (ladderXPosition + LADDER_WIDTH - 10)) // Alinhado com escada
			{
				if(eddieAreaOffset == i) // Escada no mesmo nivel que Eddie
				{
					return UP;
				}
				if(eddieAreaOffset == i + 1) // Escada logo abaixo do Eddie
				{
					return DOWN;
				}
			}
			
		}		
	}
	return NONE;
	
}

void handleEddieJump(Image* eddie)
{
	// Tratamento do pulo
		if(jumpHeight>0 && jumpHeight < JUMP_LIMIT) 
		{
			if((joyDir == RIGHT || joyDir == LEFT) && longJump == 0)
			{
				longJumpDir = joyDir;
				longJump = 20;
			}
			if(longJumpDir != NONE) // Soltou o joystick no meio do pulo long
			{
					if(longJumpDir == RIGHT)
					{
						eddie->x++;
						if(eddie->x > 128 - EDDIE_SHIRT_WIDTH)
						{
							eddie->x = 128 - EDDIE_SHIRT_WIDTH;
							eddie->isMoving = false;
						}
						else
						{
							eddie->isMoving = true;
						}		
						eddie->dirX = RIGHT;
					}
					if(longJumpDir == LEFT)
					{
						eddie->x--;
						if(eddie->x < 1)
						{
							eddie->x = 1;
							eddie->isMoving = false;
						}
						else
						{
							eddie->isMoving = true;
						}	
						eddie->dirX = LEFT;
					}	
			}
			eddie->y--;
			eddie->dirY = RIGHT; // Para cimam nao da pra usar UP, pois na funcao draw usa dirY para somar com indice i.
			jumpHeight++;
			eddie->needsUpdate = true;
			eddie->isMoving = true;
		}
		else if (jumpHeight >= JUMP_LIMIT && jumpHeight < JUMP_LIMIT*2 - 1) // Eddie alcancou o limite do pulo/ esta caindo do pulo
		{
			eddie->y++;
			eddie->dirY = LEFT; // Para baixo
			jumpHeight++;	//TODO: arrumar outro nome pra isso
			if(longJump != 0)
			{
				eddie->y--;
				eddie->dirY = NONE; //Nao cai mais
				jumpHeight--;	//TODO: arrumar outro nome pra isso
				longJump--;
			}
			if(longJumpDir != NONE) // Soltou o joystick no meio do pulo long
			{
					if(longJumpDir == RIGHT)
					{
						eddie->x++;
						if(eddie->x > 128 - EDDIE_SHIRT_WIDTH)
						{
							eddie->x = 128 - EDDIE_SHIRT_WIDTH;
							eddie->isMoving = false;
						}
						else
						{
							eddie->isMoving = true;
						}		
						eddie->dirX = RIGHT;
					}
					if(longJumpDir == LEFT)
					{
						eddie->x--;
						if(eddie->x < 1)
						{
							eddie->x = 1;
							eddie->isMoving = false;
						}
						else
						{
							eddie->isMoving = true;
						}	
						eddie->dirX = LEFT;
					}	
			}
			eddie->needsUpdate = true;
			eddie->isMoving = true;
		}
		else if(jumpHeight == 2*JUMP_LIMIT - 1 )
		{
			eddie->dirY = NONE;
			jumpHeight = 0;
			eddie->isMoving = false;
			longJump = 0;
			longJumpDir = NONE;
			eddie->needsUpdate = true;
		}
}

void handleEddieMovement(Image* eddie)
{
	if(jumpHeight != 0)
	{
		return;
	}
	if(joyMoving)
		{
			if (joyDir == UP && eddieCanGoToLadder(eddie->x,eddie->areaOffset) == UP)
			{
				clearEddie(*eddie);
				eddie->areaOffset++;
				eddie->y = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (eddie->areaOffset);
			}
			if (joyDir == DOWN && eddieCanGoToLadder(eddie->x,eddie->areaOffset) == DOWN)
			{
				clearEddie(*eddie);
				eddie->areaOffset--;
				eddie->y = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (eddie->areaOffset);
			}
			if(joyDir == RIGHT)
			{
				eddie->x++;
				if(eddie->x > 128 - EDDIE_SHIRT_WIDTH)
				{
					eddie->x = 128 - EDDIE_SHIRT_WIDTH;
					eddie->isMoving = false;
				}
				else
				{
					eddie->isMoving = true;
				}		
				eddie->dirX = RIGHT;
			}
			if(joyDir == LEFT)
			{
				eddie->x--;
				if(eddie->x < 1)
				{
					eddie->x = 1;
					eddie->isMoving = false;
				}
				else
				{
					eddie->isMoving = true;
				}	
				eddie->dirX = LEFT;
			}			
			eddie->needsUpdate = true;
			stopedMoving = false;
			if( (joyDir == UP && eddieCanGoToLadder(eddie->x,eddie->areaOffset) != UP) || (joyDir == DOWN && eddieCanGoToLadder(eddie->x,eddie->areaOffset) != DOWN))
			{
				eddie->isMoving = false;
				stopedMoving = true;
			}
		}
		else
		{
			eddie->isMoving = false;
			eddie->needsUpdate = false;
			if(!stopedMoving)
			{
				eddie->needsUpdate = true;
				stopedMoving = true;
			}
		}
}

void drawEddie(Image eddie)
{
	int eddieTopOffset,eddieInitialY;
	int i, j = 0;
	int deltay = 0;
	int startToCutEddie = 2; //Problema <
	bool first = true;
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
			flipVert(eddie_shirt_jumping, EDDIE_SHIRT_JUMPING_HEIGHT, EDDIE_SHIRT_JUMPING_WIDTH);
			flipVert(eddie_body2, EDDIE_BODY2_HEIGHT, EDDIE_BODY2_WIDTH);
			flipVert(eddie_body_moving, EDDIE_BODY_MOVING_HEIGHT, EDDIE_BODY_MOVING_WIDTH);
			flipVert(eddie_body_moving2, EDDIE_BODY_MOVING2_HEIGHT, EDDIE_BODY_MOVING2_WIDTH);
		}	
		eddieTopOffset = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (eddie.areaOffset);
		eddieInitialY = eddie.y;
		if(eddie.dirY != LEFT) // Eddie nao esta caindo do pulo, pode imprimir na ordem normal
		{
			if(eddie.dirY == RIGHT)
			{
				if(eddie.dirX == RIGHT)
				{
					eddie.x--;	
					eddie.y++;					
					clearEddie(eddie);
					eddie.x++;
					eddie.y--;
				}
				if(eddie.dirX == LEFT)
				{
					eddie.x++;	
					clearEddie(eddie);
					eddie.x--;
					eddie.y++;
					
					clearEddie(eddie);
					
					eddie.y--;
				}
				else
				{
					eddie.y++;					
					clearEddie(eddie);
					eddie.y--;
				}
				
				
			}
			eddie.colorIndex = EDDIE_HAT;
			eddie.data = eddie_hat;
			eddie.width = EDDIE_HAT_WIDTH;
			eddie.height = EDDIE_HAT_HEIGHT;
			draw(eddie); // Desenha chapeu
			
			eddie.colorIndex = EDDIE_BODY;
			eddie.data = eddie_body1;
			eddie.width = EDDIE_BODY1_WIDTH;
			eddie.height = EDDIE_BODY1_HEIGHT;
			eddie.y += EDDIE_HAT_HEIGHT;
			draw(eddie); // Desenha cabeca
			
			eddie.colorIndex = EDDIE_SHIRT;
			if(eddieInitialY < eddieTopOffset - startToCutEddie ) 
			{
				if(first)
				{
					eddie.data = eddie_shirt;
					eddie.width = EDDIE_SHIRT_WIDTH;
					eddie.height = EDDIE_SHIRT_HEIGHT + 2;
					eddie.y += EDDIE_BODY1_HEIGHT;
					clear(eddie); // Desenha camisa
					eddie.data = eddie_shirt_jumping;
					eddie.width = EDDIE_SHIRT_WIDTH;
					eddie.height = EDDIE_SHIRT_JUMPING_HEIGHT;
					draw(eddie); // Desenha camisa
					eddie.y += EDDIE_SHIRT_HEIGHT;
					eddie.colorIndex = EDDIE_BODY;			
					eddie.data = eddie_body2;
					eddie.width = EDDIE_BODY2_WIDTH;
					eddie.height = EDDIE_BODY2_HEIGHT;
					eddie.y -= 2;
					draw(eddie);
					first = false;
				}
				else
				{
					eddie.y += EDDIE_BODY1_HEIGHT;
					eddie.data = eddie_shirt_jumping;
					eddie.width = EDDIE_SHIRT_WIDTH;
					eddie.height = EDDIE_SHIRT_JUMPING_HEIGHT;
					draw(eddie); // Desenha camisa
					eddie.y += EDDIE_SHIRT_JUMPING_HEIGHT;
					eddie.colorIndex = EDDIE_BODY;			
					eddie.data = eddie_body2;
					eddie.width = EDDIE_BODY2_WIDTH;
					eddie.height = EDDIE_BODY2_HEIGHT;
					clear(eddie);
					draw(eddie);
				}
				
			}
			else
			{
				eddie.data = eddie_shirt;
				eddie.width = EDDIE_SHIRT_WIDTH;
				eddie.height = EDDIE_SHIRT_HEIGHT;
				eddie.y += EDDIE_BODY1_HEIGHT;
				draw(eddie); // Desenha camisa
				eddie.y += EDDIE_SHIRT_HEIGHT;
				eddie.colorIndex = EDDIE_BODY;
			
				eddie.data = eddie_body2;
				eddie.width = EDDIE_BODY2_WIDTH;
				eddie.height = EDDIE_BODY2_HEIGHT;
				clear(eddie);
				draw(eddie);
			}			
			
			
			
		}
		else // Eddie caindo, imprime na ordem contraria(comecando dos pes)
		{
			eddie.colorIndex = EDDIE_BODY;
			eddie.data = eddie_body2;
			eddie.width = EDDIE_BODY2_WIDTH;
			eddie.height = EDDIE_BODY2_HEIGHT;
			if(eddie.dirX == RIGHT)
				{
					eddie.x--;	
					eddie.y--;					
					clearEddie(eddie);
					eddie.x++;
					eddie.y++;
				}
				if(eddie.dirX == LEFT)
				{
					eddie.x++;	
					eddie.y--;					
					clearEddie(eddie);
					eddie.x--;
					eddie.y++;
				}
				else
				{
					eddie.y--;					
					clearEddie(eddie);
					eddie.y++;
				}
			if(eddieInitialY < eddieTopOffset - startToCutEddie ) 
			{
				eddie.y += EDDIE_HAT_HEIGHT + EDDIE_BODY1_HEIGHT + EDDIE_SHIRT_JUMPING_HEIGHT;
				draw(eddie); // Desenha os pes			
				eddie.colorIndex = EDDIE_SHIRT;
				
				eddie.y -= EDDIE_SHIRT_JUMPING_HEIGHT;
				eddie.data = eddie_shirt_jumping;
				eddie.width = EDDIE_SHIRT_WIDTH;
				eddie.height = EDDIE_SHIRT_JUMPING_HEIGHT;
				
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
			else
			{		
				eddie.y--;
				clearEddie(eddie);
				eddie.y++;
				
				eddie.y += EDDIE_HAT_HEIGHT + EDDIE_BODY1_HEIGHT + EDDIE_SHIRT_HEIGHT;
				draw(eddie); // Desenha os pes
					
				eddie.colorIndex = EDDIE_SHIRT;
				
				eddie.y -= EDDIE_SHIRT_HEIGHT;
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
						
			
		}
		
		if(eddie.isMoving && eddie.dirY == NONE && jumpHeight == 0) // Eddie esta se movendo na horizontal(dirY = NONE) no chao(jumpHeight=0)
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