#include "util.h"
#include "draw.h"
#include "joy.h"

extern osMutexId mid_displayMutex;

bool eddieCollectedItem = false;
bool eddieCollidedWithEnemy = false;
uint8_t areaOfItemCollected = -1;


Image floorImage, ladderImage;

void flipVert(uint8_t  image[], uint16_t rows, uint16_t columns)
{
	int i=0,j =0;
	uint8_t temp;
	for(i=0;i<rows;i++)
	{
		for(j=0;j<columns/2;j++)
		{
			temp = image[i*columns + j];
			image[i*columns + j] = image[i*columns + columns - 1 - j];
			image[i*columns + columns - 1 - j] = temp;
		}
	}
}

void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros){
	static const char* pAscii = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	bool n = false;
	int pos = 0, d = 0;
	int64_t tmpValue = value;

	// the buffer must not be null and at least have a length of 2 to handle one
	// digit and null-terminator
	if (pBuf == NULL || len < 2)
			return;

	// a valid base cannot be less than 2 or larger than 36
	// a base value of 2 means binary representation. A value of 1 would mean only zeros
	// a base larger than 36 can only be used if a larger alphabet were used.
	if (base < 2 || base > 36)
			return;

	if (zeros > len)
		return;
	
	// negative value
	if (value < 0)
	{
			tmpValue = -tmpValue;
			value    = -value;
			pBuf[pos++] = '-';
			n = true;
	}

	// calculate the required length of the buffer
	do {
			pos++;
			tmpValue /= base;
	} while(tmpValue > 0);


	if (pos > len)
			// the len parameter is invalid.
			return;

	if(zeros > pos){
		pBuf[zeros] = '\0';
		do{
			pBuf[d++ + (n ? 1 : 0)] = pAscii[0]; 
		}
		while(zeros > d + pos);
	}
	else
		pBuf[pos] = '\0';

	pos += d;
	do {
			pBuf[--pos] = pAscii[value % base];
			value /= base;
	} while(value > 0);
}

Direction joyDir;
extern tContext sContext;
bool joyMoving;

void Entrada(void const *arg)
{
	osStatus status;
	int x;
	// char pbufx[10];
	while(1)
	{
		x = joy_read_x();
		x = x*200/0xFFF-100;		
		// intToString(x, pbufx, 10, 10, 4);
		status = osMutexWait(mid_displayMutex, osWaitForever);
		// GrStringDraw(&sContext,(char*)pbufx, -1, 10,  10, true);		
		osMutexRelease(mid_displayMutex);
		if (x > JOY_SENSITIVITY)
		{
			joyMoving = true;
			joyDir = RIGHT;
		}		
		else if(x < -JOY_SENSITIVITY)
		{
			joyMoving = true;
			joyDir = LEFT;
		}	
		else
		{
			joyMoving = false;
		}
		
	}
}

void Eddie(void const *arg)
{
	osStatus status;
	char pbufx[10];
	Image eddie;	
	bool first = true;
	eddie.x = 0;
	eddie.y = 0;
	eddie.areaOffset = 3;
	eddie.needsUpdate = true;
	eddie.isMoving = false;
	eddie.dirX = RIGHT;
	eddie.dirY = 0;
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);
		// if (first)
		// {
		// 	drawEddie(0,areaOffset,UPDATE);
		// 	first = false;
		// }
		if(eddieCollidedWithEnemy)
		{
			GrStringDraw(&sContext,"Morreu",-1,0,10,true);
			eddieCollidedWithEnemy = false;
		}
		drawEddie(eddie);
		osMutexRelease(mid_displayMutex);
		if(joyMoving)
		{
			if(joyDir == RIGHT)
			{
				eddie.x++;
				if(eddie.x > 128 - EDDIE_SHIRT_WIDTH)
				{
					eddie.x = 128 - EDDIE_SHIRT_WIDTH;
					eddie.isMoving = false;
				}
				else
				{
					eddie.isMoving = true;
				}
				
					
				eddie.dirX = RIGHT;
			}
			else if(joyDir == LEFT)
			{
				eddie.x--;
				if(eddie.x < 1)
				{
					eddie.x = 1;
					eddie.isMoving = false;
				}
				else
				{
					eddie.isMoving = true;
				}	
				eddie.dirX = LEFT;
			}
			eddie.needsUpdate = true;
			ladderImage.needsUpdate = true;
		}
		else
		{
			eddie.isMoving = false;
			eddie.needsUpdate = false;
		}
		
		
		
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
		osDelay(15);
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
	}
}

void ItensBrilhantes(void const *arg)
{
	osStatus status;
	int i,numberOfItens =3,itemTopOffset;
	Image itens[3];	
	Image item1, item2, item3;
	int xOffset[] = {10, 60, 0};
	int areaOffset[] = {1,2,3};
	char buffer[10];
	Direction dir[] = {RIGHT,RIGHT,RIGHT};
	item1.areaOffset = 0;
	item2.areaOffset = 3;
	item3.areaOffset = 2;
	itens[0] = item1;
	itens[1] = item2;
	itens[2] = item3;
	
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);	

		for (i = 0; i < numberOfItens; i++)
		{
			if (areaOfItemCollected != areaOffset[i])
			{
				itens[i].needsUpdate = true;
				itens[i].isMoving = true;
				itens[i].dirX = dir[i];
				itens[i].x = xOffset[i];
				drawItem(itens[i]);			
				// drawItem(xOffset[i], areaOffset[i], dir[i]);
			}
			
		}
		osMutexRelease(mid_displayMutex);			
		osDelay(16);		
		for (i = 0; i < numberOfItens; i++)
		{
			if(xOffset[i] == 60)
			{
				dir[i] = RIGHT;
			}
			else if (xOffset[i] == 110)
			{
				dir[i] = LEFT;
			}
			if(dir[i] == RIGHT)
			{
				xOffset[i]++;
			}
			else
			{
				xOffset[i]--;
			}
		}
		
		
		
		
		
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
	while(1)
	{
	}
}