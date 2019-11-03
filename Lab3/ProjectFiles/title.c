#include "title.h"

extern tContext sContext;
extern osMutexId context_mutex;

const uint8_t title[] = {
 0xc8,0xc8,0xc8,0x88,0x88,0x88,0x7c,0x7c,0x7c,0x30,0x30,0x30,0x60,0x60,0x60,0x80,0x80,0x80,0x74,0x74,0x74,0x2c,0x2c,0x2c,0x5c,0x5c,0x5c,0x80,0x80,0x80,0x80,0x80,0x80,0x34,0x34,0x34,0x60,0x60,0x60,0xa4,0xa4,0xa4,0xbc,0xbc,0xbc,0x7c,0x7c,0x7c,0x20,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x38,0xb8,0xb8,0xb8,0x84,0x84,0x84,0x70,0x70,0x70,0x44,0x44,0x44,0xb8,0xb8,0xb8,0x84,0x84,0x84,0x6c,0x6c,0x6c,0x40,0x40,0x40,0xb8,0xb8,0xb8,0x88,0x88,0x88,0x70,0x70,0x70,0x3c,0x3c,0x3c,0x90,0x90,0x90,0x70,0x70,0x70,0xb0,0xb0,0xb0,0x80,0x80,0x80,0x70,0x70,0x70
,0xac,0xac,0xac,0x1c,0x1c,0x1c,0x0c,0x0c,0x0c,0x20,0x20,0x20,0x9c,0x9c,0x9c,0x20,0x20,0x20,0x80,0x80,0x80,0x64,0x64,0x64,0x90,0x90,0x90,0x24,0x24,0x24,0x08,0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x48,0x48,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x38,0x90,0x90,0x90,0x14,0x14,0x14,0x0c,0x0c,0x0c,0x34,0x34,0x34,0x90,0x90,0x90,0x08,0x08,0x08,0x88,0x88,0x88,0x60,0x60,0x60,0x90,0x90,0x90,0x0c,0x0c,0x0c,0x84,0x84,0x84,0x60,0x60,0x60,0x90,0x90,0x90,0x70,0x70,0x70,0x70,0x70,0x70,0x0c,0x0c,0x0c,0x10,0x10,0x10
,0xd0,0xd0,0xd0,0x9c,0x9c,0x9c,0x8c,0x8c,0x8c,0x44,0x44,0x44,0xb8,0xb8,0xb8,0x9c,0x9c,0x9c,0xc0,0xc0,0xc0,0x50,0x50,0x50,0x38,0x38,0x38,0x90,0x90,0x90,0x60,0x60,0x60,0x0c,0x0c,0x0c,0x00,0x00,0x00,0x48,0x48,0x48,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x38,0xc0,0xc0,0xc0,0x98,0x98,0x98,0x84,0x84,0x84,0x4c,0x4c,0x4c,0x8c,0x8c,0x8c,0x08,0x08,0x08,0x8c,0x8c,0x8c,0x60,0x60,0x60,0x90,0x90,0x90,0x08,0x08,0x08,0x84,0x84,0x84,0x60,0x60,0x60,0x90,0x90,0x90,0x70,0x70,0x70,0xbc,0xbc,0xbc,0x94,0x94,0x94,0x80,0x80,0x80
,0xac,0xac,0xac,0x24,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x24,0x9c,0x9c,0x9c,0x28,0x28,0x28,0x84,0x84,0x84,0x4c,0x4c,0x4c,0x00,0x00,0x00,0x14,0x14,0x14,0x74,0x74,0x74,0x50,0x50,0x50,0x00,0x00,0x00,0x48,0x48,0x48,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x38,0x94,0x94,0x94,0x1c,0x1c,0x1c,0x14,0x14,0x14,0x34,0x34,0x34,0x90,0x90,0x90,0x08,0x08,0x08,0x8c,0x8c,0x8c,0x60,0x60,0x60,0x90,0x90,0x90,0x08,0x08,0x08,0x84,0x84,0x84,0x60,0x60,0x60,0x90,0x90,0x90,0x70,0x70,0x70,0x74,0x74,0x74,0x14,0x14,0x14,0x14,0x14,0x14
,0xa0,0xa0,0xa0,0x0c,0x0c,0x0c,0x00,0x00,0x00,0x1c,0x1c,0x1c,0x94,0x94,0x94,0x0c,0x0c,0x0c,0x74,0x74,0x74,0x58,0x58,0x58,0x5c,0x5c,0x5c,0x74,0x74,0x74,0x74,0x74,0x74,0x28,0x28,0x28,0x00,0x00,0x00,0x48,0x48,0x48,0x7c,0x7c,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x34,0x34,0x34,0xb0,0xb0,0xb0,0x74,0x74,0x74,0x68,0x68,0x68,0x44,0x44,0x44,0xb0,0xb0,0xb0,0x78,0x78,0x78,0x6c,0x6c,0x6c,0x40,0x40,0x40,0xb0,0xb0,0xb0,0x78,0x78,0x78,0x6c,0x6c,0x6c,0x40,0x40,0x40,0x8c,0x8c,0x8c,0x6c,0x6c,0x6c,0xa4,0xa4,0xa4,0x74,0x74,0x74,0x64,0x64,0x64
,0x30,0x30,0x30,0x04,0x04,0x04,0x00,0x00,0x00,0x08,0x08,0x08,0x2c,0x2c,0x2c,0x04,0x04,0x04,0x20,0x20,0x20,0x1c,0x1c,0x1c,0x38,0x38,0x38,0x48,0x48,0x48,0x24,0x24,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x14,0x14,0x24,0x24,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x44,0x44,0x44,0x48,0x48,0x48,0x40,0x40,0x40,0x18,0x18,0x18,0x40,0x40,0x40,0x48,0x48,0x48,0x14,0x14,0x14,0x08,0x08,0x08,0x40,0x40,0x40,0x48,0x48,0x48,0x18,0x18,0x18,0x08,0x08,0x08,0x28,0x28,0x28,0x20,0x20,0x20,0x48,0x48,0x48,0x48,0x48,0x48,0x40,0x40,0x40
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x18,0x0c,0x08,0x54,0x34,0x24,0x58,0x34,0x28,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x38,0x28,0x2c,0x1c,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x10,0x08,0x54,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x28,0x28,0x18,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x10,0x0c,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x58,0x34,0x24,0x24,0x14,0x10,0x00,0x00,0x00,0x00,0x00,0x00
,0x38,0x24,0x18,0xc4,0x78,0x54,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0x68,0x40,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x24,0x18,0xc8,0x7c,0x58,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0x60,0x3c,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0x28,0x1c,0xcc,0x7c,0x58,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0xd0,0x80,0x5c,0x58,0x34,0x24,0x00,0x00,0x00,0x00,0x00,0x00

};


uint32_t titleOneChannel[TITLE_PIXELS];
void drawTitle(void)
{
    uint16_t i, j;

	j = 0;
    for (i = 0; i < TITLE_PIXELS; i++)
    {
        titleOneChannel[j] = (title[3*i]<<16) + (title[3*i+1]<<8) + (title[3*i+2]);
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);
    
	
    for (i = 0; i < TITLE_HEIGHT; i++)
    {
        for (j = 0; j < TITLE_WIDTH; j++)
        {
            GrContextForegroundSet(&sContext, titleOneChannel[i*TITLE_WIDTH + j]);
            GrPixelDraw(&sContext,j+43,i+10);
        }
    }
}

void Title(void)
{
	osMutexWait(context_mutex, osWaitForever);
	drawTitle();
	osMutexRelease(context_mutex);
}