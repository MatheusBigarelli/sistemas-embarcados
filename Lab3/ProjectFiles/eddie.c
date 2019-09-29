#include "eddie.h"

unsigned char eddie[] = {
 0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x28,0x44,0x24,0x2c,0x4c,0x10,0x10,0x20,0x00,0x00,0x00
,0x00,0x00,0x00,0x38,0x3c,0x6c,0x84,0x90,0xe4,0x88,0x98,0xf0,0x64,0x70,0xb8,0x10,0x14,0x20
,0x00,0x00,0x00,0x0c,0x10,0x10,0xd0,0xd0,0x54,0xe4,0xe4,0x58,0x98,0x98,0x40,0x14,0x14,0x08
,0x00,0x00,0x00,0x08,0x0c,0x04,0xcc,0xd4,0x40,0xe0,0xe8,0x48,0x94,0x9c,0x30,0x14,0x14,0x04
,0x0c,0x1c,0x0c,0x38,0x7c,0x38,0x50,0xa0,0x44,0x3c,0x70,0x30,0x40,0x84,0x38,0x20,0x4c,0x20
,0x24,0x58,0x24,0x48,0xa4,0x48,0x3c,0x8c,0x3c,0x00,0x00,0x00,0x28,0x58,0x28,0x44,0x9c,0x44
,0x24,0x58,0x24,0x48,0xa4,0x48,0x3c,0x8c,0x3c,0x00,0x00,0x00,0x28,0x58,0x28,0x44,0x9c,0x44
,0x24,0x58,0x24,0x48,0xa4,0x48,0x3c,0x8c,0x3c,0x00,0x00,0x00,0x28,0x58,0x28,0x44,0x9c,0x44
,0x24,0x58,0x24,0x48,0xa4,0x48,0x3c,0x8c,0x3c,0x00,0x00,0x00,0x18,0x34,0x18,0x28,0x60,0x28
,0x24,0x58,0x24,0x48,0xa4,0x48,0x44,0x9c,0x44,0x24,0x54,0x24,0x08,0x14,0x08,0x08,0x14,0x08
,0x14,0x30,0x14,0x40,0x90,0x40,0x48,0xa4,0x48,0x48,0xa0,0x48,0x44,0x98,0x44,0x2c,0x64,0x2c
,0x00,0x00,0x00,0x10,0x20,0x0c,0xa0,0xb8,0x3c,0xb0,0xc8,0x44,0x58,0x70,0x28,0x04,0x08,0x04
,0x00,0x00,0x00,0x04,0x04,0x00,0xc0,0xc0,0x3c,0xd4,0xd4,0x40,0x60,0x60,0x1c,0x00,0x00,0x00
,0x00,0x00,0x00,0x04,0x04,0x00,0xc0,0xc0,0x3c,0xd4,0xd4,0x40,0x70,0x70,0x20,0x20,0x20,0x08
,0x00,0x00,0x00,0x04,0x04,0x04,0xc0,0xc0,0x3c,0xd0,0xd4,0x40,0xcc,0xd0,0x40,0xc4,0xc4,0x3c};
 

void drawEddie(tContext sContext, int x, int y)
{
    int i, j;
	int numberOfPixels;
    uint32_t eddieOneChannel[90];

	numberOfPixels = sizeof(eddie)/sizeof(unsigned char);
	j = 0;
    for (i = 0; i < numberOfPixels - 3; i+=3)
    {
        eddieOneChannel[j] = (eddie[i]<<16) + (eddie[i+1]<<8) + (eddie[i+2]);
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);
    
	
    for (i = 0; i < 15; i++)
    {
        for (j = 0; j < 6; j++)
        {
            GrContextForegroundSet(&sContext, eddieOneChannel[i*6 + j]);
            GrPixelDraw(&sContext,j+x+64,i-y+64);
        }
    }
}