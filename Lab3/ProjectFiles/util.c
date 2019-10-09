#include "util.h"
#include "draw.h"

extern osMutexId mid_displayMutex;

void flipVert(uint32_t image[], uint16_t rows, uint16_t columns)
{
	int i=0,j =0;
	uint32_t temp;
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

void Entrada(void const *arg)
{
	osStatus status;
	while(1)
	{
		
	}
}

void Eddie(void const *arg)
{
	osStatus status;
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);
		drawEddie(30,3);
		osMutexRelease(mid_displayMutex);
	}
}

void Inimigos(void const *arg)
{
	osStatus status;
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);
		drawSneaker(0,0);
		drawSneaker(70,3);
		drawBoss(20,4);
		osMutexRelease(mid_displayMutex);
	}
}

void ItensBrilhantes(void const *arg)
{
	osStatus status;
	int xOffset = 0;
	uint8_t delta = 0;
	while(1)
	{
		status = osMutexWait(mid_displayMutex, osWaitForever);		
		xOffset += delta;
		drawItem(xOffset, 1,0,delta);
		osMutexRelease(mid_displayMutex);
		if(xOffset == (121))
		{
			delta = -1;
		}
		if(xOffset == 0)
		{
			delta = 1;
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