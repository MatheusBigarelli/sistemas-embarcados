#include "threads.h"

extern osMutexId mid_displayMutex;

bool eddieCollectedItem = false;
bool eddieCollidedWithEnemy = false;
uint8_t areaOfItemCollected = -1;
bool stopedMoving = false; // Usado para evitar flicker ao atualizar as pernas do Eddie


Image floorImage, ladderImage;
Direction joyDir = NONE;
extern tContext sContext;
bool joyMoving;
extern int jumpHeight;

extern osThreadId Entrada_tid;
extern osThreadId Eddie_tid;
extern osThreadId Inimigos_tid;
extern osThreadId ItensBrilhantes_tid;
extern osThreadId PainelDeInstrumentos_tid;
extern osThreadId Saida_tid;

void Entrada(void const *arg)
{
	osStatus status;
	int x,y;
	// char pbufx[10];
	while(1)
	{
		x = joy_read_x();
		y = joy_read_y();
		if(button_read_s2())
		{
			if(jumpHeight == 0)
			{
					jumpHeight = 1;
			}
			
		}
		x = x*200/0xFFF-100;		
		y = y*200/0xFFF-100;
		// intToString(x, pbufx, 10, 10, 4);
		status = osMutexWait(mid_displayMutex, osWaitForever);
		// GrStringDraw(&sContext,(char*)pbufx, -1, 10,  10, true);		
		osMutexRelease(mid_displayMutex);
		joyMoving = false;
		joyDir = NONE;
		if (x > JOY_SENSITIVITY_X)
		{			
			joyMoving = true;
			joyDir = RIGHT;
			
		}		
		if(x < -JOY_SENSITIVITY_X)
		{
			joyMoving = true;
			joyDir = LEFT;
		}	
		if (y > JOY_SENSITIVITY_Y)
		{
			joyMoving = true;
			joyDir = UP;
		}
		if(y <  -JOY_SENSITIVITY_Y)
		{
			joyMoving = true;
			joyDir = DOWN;
		}
		osSignalSet(Eddie_tid, SIGNAL_INPUT_READY);
		osSignalSet(Inimigos_tid, SIGNAL_INPUT_READY);
		osSignalSet(ItensBrilhantes_tid, SIGNAL_INPUT_READY);
	}
}

void Eddie(void const *arg)
{
	osStatus status;
	osEvent evt;
	int initialXPosition = 0;
	int initialAreaOffset = 3;
	char pbufx[10];
	Image eddie;	
	bool first = true;
	
	
	eddie.x = initialXPosition;
	eddie.areaOffset = initialAreaOffset;	
	eddie.y = (127 - FLOOR_HEIGHT) - EDDIE_HEIGHT - (LADDER_HEIGHT + FLOOR_HEIGHT) * (eddie.areaOffset);
	eddie.needsUpdate = true;
	eddie.isMoving = false;
	eddie.dirX = NONE;
	eddie.dirY = NONE;
	while(1)
	{
		osSignalWait(SIGNAL_INPUT_READY, osWaitForever); // Espera ter alguma coisa na entrada
		osSignalClear(Eddie_tid, SIGNAL_INPUT_READY);
		
		status = osMutexWait(mid_displayMutex, osWaitForever);
		if(eddieCollidedWithEnemy)
		{
			eddieCollidedWithEnemy = false;
			clearEddie(eddie);
			eddie.x--;
			clearEddie(eddie);
			eddie.y--;
			clearEddie(eddie);
			eddie.x++;
			clearEddie(eddie);
			eddie.y++;
			eddie.x = initialXPosition;
			eddie.areaOffset = initialAreaOffset;
		}
		if(eddieCollectedItem)
		{
			blinkDisplay();
			eddieCollectedItem = false;
		}
		drawEddie(eddie);
		
		handleEddieJump(&eddie);
		handleEddieMovement(&eddie);
		osMutexRelease(mid_displayMutex);
		osSignalSet(Saida_tid, SIGNAL_EDDIE_READY);
	}
}


void Inimigos(void const *arg)
{
	osStatus status;
	Image enemy;
	int xOffset1 = 0;
	Direction dir1 = RIGHT;
	int xOffset2 = 20;
	Direction dir2 = RIGHT;
	enemy.x = enemy.y = 0;
	enemy.needsUpdate = true;
	enemy.isMoving = false;
	enemy.dirX = RIGHT;
	enemy.dirY = 0;
	while(1)
	{
		osSignalWait(SIGNAL_INPUT_READY, osWaitForever); // Espera ter alguma coisa na entrada
		osSignalClear(Inimigos_tid, SIGNAL_INPUT_READY);
		
		status = osMutexWait(mid_displayMutex, osWaitForever);
		enemy.x = xOffset1;
		enemy.areaOffset = 0;
		enemy.dirX = dir1;
		drawSneaker(enemy);
		enemy.areaOffset = 3;
		enemy.x = 70;
		drawSneaker(enemy);
		enemy.areaOffset = 4;
		enemy.x = xOffset2;
		enemy.dirX = dir2;
		drawBoss(enemy);
		osMutexRelease(mid_displayMutex);
		enemy.isMoving = true;
		if(xOffset1 == 0)
		{
			dir1 = RIGHT;
		}
		else if (xOffset1 == 100)
		{
			dir1 = LEFT;
		}
		if(dir1 == RIGHT)
		{
			xOffset1++;
		}
		else
		{
			xOffset1--;
		}
		
		if(xOffset2 == 20)
		{
			dir2 = RIGHT;
		}
		else if (xOffset2 == 100)
		{
			dir2 = LEFT;
		}
		if(dir2 == RIGHT)
		{
			xOffset2++;
		}
		else
		{
			xOffset2--;
		}
		osSignalSet(Saida_tid, SIGNAL_ENEMIES_READY);
	}
}

void ItensBrilhantes(void const *arg)
{
	osStatus status;
	int i,numberOfItens =2,itemTopOffset;
	Image itens[2];	
	Image item1, item2;
	char buffer[10];
	item1.areaOffset = 0;
	item1.x = 10;
	item1.dirX = RIGHT;
	item2.areaOffset = 3;
	item2.x = 60;
	item2.dirX = RIGHT;
	itens[0] = item1;
	itens[1] = item2;
	
	while(1)
	{
		osSignalWait(SIGNAL_INPUT_READY, osWaitForever); // Espera ter alguma coisa na entrada
		osSignalClear(ItensBrilhantes_tid, SIGNAL_INPUT_READY);
		
		status = osMutexWait(mid_displayMutex, osWaitForever);	

		for (i = 0; i < numberOfItens; i++)
		{
			if(itens[i].collected)continue;
			if (areaOfItemCollected != itens[i].areaOffset)
			{
				itens[i].needsUpdate = true;
				itens[i].isMoving = true;
				drawItem(&itens[i]);			
			}
			else
			{				
				collectItem(&itens[i]);
			}
			
		}
		updateItens(&itens[0], &itens[1]);	
		osMutexRelease(mid_displayMutex);		
		osSignalSet(Saida_tid, SIGNAL_ITENS_READY);	
	}		
}

void PainelDeInstrumentos(void const *arg)
{
	osStatus status;	

	floorImage.needsUpdate = true;		
	ladderImage.needsUpdate = true;
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);
		drawFloor(floorImage);
		floorImage.needsUpdate = false;
		drawLadder(ladderImage);
		ladderImage.needsUpdate = false;
		drawScore();
		
		
		osMutexRelease(mid_displayMutex);
	}
}

void Saida(void const *arg)
{
	osStatus status;
	uint8_t* song;
	uint8_t songMario[SONG_SIZE] = {"^En,^En.^En_^Cn,^En_^Gn_-Nn_-Gn_-Nn_-Nn_"};
	buzzer_vol_set(750);
	
	while(1)
	{
		osSignalWait(SIGNAL_EDDIE_READY, osWaitForever); // Espera thread do Eddie
		osSignalClear(Saida_tid, SIGNAL_EDDIE_READY);
		osSignalWait(SIGNAL_ENEMIES_READY, osWaitForever); // Espera thread dos inimigos
		osSignalClear(Saida_tid, SIGNAL_ENEMIES_READY);
		osSignalWait(SIGNAL_ITENS_READY, osWaitForever); // Espera thread dos itens
		osSignalClear(Saida_tid, SIGNAL_ITENS_READY);
		
		song = songMario;
		//playSong(song);
	}
}