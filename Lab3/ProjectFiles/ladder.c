#include "ladder.h"

extern tContext sContext;
extern osMutexId context_mutex;

const uint8_t ladder[] = {
0x78,0x90,0xac,0x78,0x90,0xb0,0x60,0x6c,0x80,0x24,0x14,0x14,0x24,0x10,0x10,0x24,0x10,0x10,0x24,0x10,0x10,0x20,0x0c,0x0c,0x28,0x18,0x18,0x64,0x74,0x8c,0x78,0x90,0xb0,0x74,0x8c,0xac
,0x64,0x9c,0xc0,0x68,0xa0,0xc4,0x48,0x70,0x88,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x50,0x78,0x98,0x68,0xa0,0xc4,0x64,0x9c,0xc0
,0x68,0x9c,0xc0,0x68,0x9c,0xc4,0x4c,0x74,0x90,0x10,0x18,0x20,0x0c,0x14,0x18,0x0c,0x14,0x18,0x0c,0x14,0x18,0x0c,0x14,0x18,0x10,0x1c,0x24,0x54,0x80,0x9c,0x68,0x9c,0xc0,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc0,0x60,0x94,0xb4,0x54,0x80,0x9c,0x54,0x80,0x9c,0x54,0x80,0x9c,0x54,0x80,0x9c,0x54,0x80,0x9c,0x54,0x80,0xa0,0x64,0x94,0xb8,0x68,0x9c,0xc0,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc0,0x50,0x78,0x94,0x18,0x24,0x2c,0x14,0x20,0x28,0x14,0x20,0x28,0x14,0x20,0x28,0x14,0x20,0x28,0x18,0x28,0x30,0x54,0x80,0xa0,0x68,0x9c,0xc0,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc4,0x48,0x70,0x88,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x08,0x50,0x78,0x94,0x68,0x9c,0xc4,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc4,0x48,0x6c,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x08,0x50,0x78,0x94,0x68,0x9c,0xc4,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc0,0x58,0x84,0xa4,0x30,0x48,0x5c,0x2c,0x48,0x58,0x30,0x48,0x58,0x30,0x48,0x58,0x2c,0x48,0x58,0x30,0x4c,0x60,0x5c,0x88,0xa8,0x68,0x9c,0xc0,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc0,0x5c,0x8c,0xb0,0x44,0x6c,0x84,0x44,0x68,0x80,0x44,0x68,0x80,0x44,0x68,0x80,0x44,0x68,0x80,0x48,0x6c,0x84,0x60,0x90,0xb4,0x68,0x9c,0xc0,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc4,0x48,0x70,0x88,0x04,0x04,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x0c,0x0c,0x50,0x7c,0x98,0x68,0x9c,0xc4,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc4,0x48,0x70,0x88,0x00,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x0c,0x50,0x78,0x98,0x68,0x9c,0xc4,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc4,0x48,0x6c,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x08,0x50,0x78,0x94,0x68,0x9c,0xc4,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc0,0x54,0x80,0x9c,0x24,0x38,0x44,0x20,0x34,0x40,0x20,0x34,0x40,0x20,0x34,0x40,0x20,0x34,0x40,0x24,0x3c,0x48,0x58,0x84,0xa4,0x68,0x9c,0xc0,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc0,0x60,0x90,0xb4,0x50,0x78,0x94,0x4c,0x78,0x90,0x4c,0x78,0x90,0x4c,0x78,0x90,0x4c,0x74,0x90,0x50,0x78,0x94,0x60,0x94,0xb4,0x68,0x9c,0xc0,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc4,0x48,0x70,0x8c,0x08,0x0c,0x10,0x04,0x08,0x08,0x04,0x08,0x08,0x04,0x08,0x08,0x04,0x08,0x08,0x08,0x10,0x14,0x50,0x7c,0x98,0x68,0x9c,0xc0,0x64,0x98,0xbc
,0x68,0x9c,0xc0,0x68,0x9c,0xc4,0x48,0x70,0x88,0x00,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x0c,0x50,0x78,0x98,0x68,0x9c,0xc4,0x64,0x98,0xbc
,0x64,0x9c,0xc0,0x68,0xa0,0xc4,0x48,0x70,0x88,0x00,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0x08,0x50,0x7c,0x98,0x68,0xa0,0xc4,0x64,0x9c,0xc0
	
};


uint32_t ladderOneChannel[LADDER_PIXELS];
void drawLadder(uint16_t x, uint16_t y)
{
    uint8_t i, j;
	j = 0;
    for (i = 0; i < LADDER_PIXELS - 3; i+=3)
    {
        ladderOneChannel[j] = (ladder[i]<<16) + (ladder[i+1]<<8) + (ladder[i+2]);
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);
    
	
    for (i = 0; i < LADDER_HEIGHT; i++)
    {
        for (j = 0; j < LADDER_WIDTH; j++)
        {
            GrContextForegroundSet(&sContext, ladderOneChannel[i*LADDER_WIDTH + j]);
            GrPixelDraw(&sContext,j+x,i+y);
        }
    }
}

void Ladder(void)
{
	uint8_t i;
	uint16_t x[NUM_LADDERS] = {90, 30, 60}, y[NUM_LADDERS] = {90, 70, 30};
	while(1)
	{
		for (i = 0; i < NUM_LADDERS; i++)
		{
			osMutexWait(context_mutex, osWaitForever);
			drawLadder(x[i], y[i]);
			osMutexRelease(context_mutex);
		}
	}
}