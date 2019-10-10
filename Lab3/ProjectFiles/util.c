#include "util.h"
#include "draw.h"
#include "joy.h"
#include "images.h"

extern osMutexId mid_displayMutex;

bool eddieCollectedItem = false;
bool eddieCollidedWithEnemy = false;
uint8_t areaOfItemCollected;

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

Direction joyDir = UPDATE;
extern tContext sContext;

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
		if (x > 1)
		{
			joyDir = RIGHT;
		}		
		else if(x < -JOY_SENSITIVITY)
		{
			joyDir = LEFT;
		}
		else
		{
			joyDir = NONE;
		}		
	}
}

void Eddie(void const *arg)
{
	osStatus status;
	int xOffset = 0;
	int areaOffset = 2;
	char pbufx[10];
	bool first = true;
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);
		if (first)
		{
			drawEddie(0,areaOffset,UPDATE);
			first = false;
		}
		if(eddieCollidedWithEnemy)
		{
			GrStringDraw(&sContext,"Morreu",-1,0,10,true);
			eddieCollidedWithEnemy = false;
		}
		drawEddie(xOffset,areaOffset,joyDir);
		osMutexRelease(mid_displayMutex);
		if(joyDir == RIGHT)
		{
			xOffset++;
			if(xOffset > 128 - 10)
				xOffset = 128 - 10;
		}
		else if(joyDir == LEFT)
		{

			xOffset--;
			if(xOffset < 0)
				xOffset = 0;
		}
	}
}

void Inimigos(void const *arg)
{
	osStatus status;
	int xOffset1 = 0;
	Direction dir1 = NONE;
	int xOffset2 = 20;
	Direction dir2 = NONE;
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);
		drawSneaker(xOffset1,0,dir1);
		drawSneaker(70,3,UPDATE);
		drawBoss(xOffset2,4,dir2);
		osMutexRelease(mid_displayMutex);
		osDelay(5);
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
	int xOffset[] = {60, 60, 60};
	int areaOffset[] = {1,2,3};
	char buffer[10];
	Direction dir[] = {NONE,NONE,NONE};
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);	
		if(eddieCollectedItem)
		{
			intToString(areaOfItemCollected,buffer,10,10,1);
			GrStringDraw(&sContext,(char*)buffer,-1,0,10,true);
			eddieCollectedItem = false;
			for (i = 0; i < numberOfItens; i++)
			{
				if(areaOfItemCollected == areaOffset[i])
				{
					itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT) * (areaOffset[i]) + 1 + 10;
					clear(ITEM,ITEM_HEIGHT,ITEM_WIDTH,xOffset[i],itemTopOffset);
				}
			}
			
		}
		for (i = 0; i < numberOfItens; i++)
		{
			if (areaOfItemCollected != areaOffset[i])
			{
				drawItem(xOffset[i], areaOffset[i], dir[i]);
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
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);
		drawFloor();
		drawLadder();
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