#include "util.h"
#include "draw.h"
#include "joy.h"

extern osMutexId mid_displayMutex;

bool eddieCollectedItem = false;
bool eddieCollidedWithEnemy = false;
uint8_t areaOfItemCollected = -1;
bool stopedMoving = false; // Usado para evitar flicker ao atualizar as pernas do Eddie


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

Direction joyDir = NONE;
extern tContext sContext;
bool joyMoving;
extern int jumpHeight;

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
		
		
	}
}

void Eddie(void const *arg)
{
	osStatus status;
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
			eddieCollectedItem = false;
		}
		drawEddie(eddie);
		
		handleEddieJump(&eddie);
		handleEddieMovement(&eddie);
		osMutexRelease(mid_displayMutex);
		
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



uint16_t notes[2][14] = {
    {sAb(1), sA(1), sBb(1), sB(1), 0, sC(1), sDb(1), sD(1), sEb(1), sE(1), 0, sF(1), sGb(1), sG(1)},
    {sAb(2), sA(2), sBb(2), sB(2), 0, sC(2), sDb(2), sD(2), sEb(2), sE(2), 0, sF(2), sGb(2), sG(2)},
};

uint16_t getNote(uint8_t *ch)
{
    uint8_t scale = 0;
    if (ch[0] == '^')
        scale = 1;


    if (ch[1] >= 'A' && ch[1] <= 'G')
    {
        if (ch[2] == 'b')
            return notes[scale][2 * (ch[1] - 'A')];
        else if (ch[2] == 'n')
            return notes[scale][2 * (ch[1] - 'A')+1];
    }

    return 0;
}


uint32_t getPause(uint8_t ch)
{
    switch (ch) {
    case '+':
        return 0;
    case ',':
        return 500;
    case '.':
        return 1000;
    case '_':
        return 2000;
    default:
        return 5;
    }
}

void playNote(uint16_t note, uint32_t dur)
{
	buzzer_per_set(note);
	buzzer_write(true);
	osDelay(dur);
}

void playSong(uint8_t *song) {
    uint32_t note;
    uint32_t dur;
    uint32_t pause;
	
	dur = 1000;

    
     // A song is a collection of tones where each tone is
     // a note, duration and pause, e.g.
     // "E2,F4,"
     

    while(*song != '\0') {
        note = getNote(song);
		song += 3;
		
        pause = getPause(*song++);

        playNote(note, dur);
		
		buzzer_write(false);
		osDelay(pause);
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
		song = songMario;
		//playSong(song);
	}
}