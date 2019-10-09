#include "item.h"

extern int16_t itens_x[], itens_y[];
extern osMutexId context_mutex;
extern tContext sContext;


const uint8_t item[] = {
 0x58,0x34,0x34,0xb4,0x6c,0x6c,0xcc,0x7c,0x7c,0x84,0x50,0x50,0xb4,0x6c,0x6c,0xcc,0x7c,0x7c,0x84,0x50,0x50
,0xb8,0x70,0x70,0xe0,0x88,0x88,0xe0,0x88,0x88,0xdc,0x88,0x88,0xe0,0x88,0x88,0xe0,0x88,0x88,0xdc,0x84,0x84
,0x40,0x28,0x28,0x94,0x58,0x58,0xd4,0x80,0x80,0xe0,0x88,0x88,0xdc,0x84,0x84,0xb4,0x6c,0x6c,0x64,0x3c,0x3c
,0x00,0x00,0x00,0x0c,0x08,0x08,0x58,0x34,0x34,0xb8,0x70,0x70,0x84,0x50,0x50,0x20,0x14,0x14,0x00,0x00,0x00
};	


uint32_t itemOneChannel[ITEM_PIXELS];
void drawItem(int16_t x, int16_t y, int16_t last_x, int16_t last_y, uint8_t state[NUM_CHANNELS])
{
    uint16_t i, j;
	uint32_t r, g, b;
	uint32_t foreground;
	
	j = 0;
    for (i = 0; i < ITEM_PIXELS - 3; i+=3)
    {
		r = item[i];
		g = item[i+1];
		b = item[i+2];
		if (r > 10)
			r = (r+state[0]) % 256;
		if (r < 10)
			r = item[i];
		
		if (g > 10)
			g = (g+state[1]) % 256;
		if (g < 10)
			g = item[i];
		
		if (b > 10)
			b = (b+state[2]) % 256;
		if (b < 10)
			b = item[i];
		
		itemOneChannel[j] = (r << 16) + (g << 8) + b;
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);
    
	
	GrContextForegroundSet(&sContext, ClrBlack);
	
	if (last_x < x)
	{
		for (i = 0; i < ITEM_HEIGHT; i++)
		{
			GrPixelDraw(&sContext,(last_x)%128,(i+last_y)%128);
		}
	}
	
	else
	{
		for (i = 0; i < ITEM_HEIGHT; i++)
		{
			GrPixelDraw(&sContext,((ITEM_WIDTH-1)+last_x)%128,(i+last_y)%128);
		}
	}
	
    for (i = 0; i < ITEM_HEIGHT; i++)
    {
        for (j = 0; j < ITEM_WIDTH; j++)
        {
			foreground = itemOneChannel[i*ITEM_WIDTH + j];
			if (foreground > 5)
			{
				GrContextForegroundSet(&sContext, foreground);
				GrPixelDraw(&sContext,(j+x)%128,(i+y)%128);
			}
        }
    }
}


void Item(void const *args)
{
	uint8_t i, j;
	int16_t dx[] = {1, 1, -1};
	int16_t last_x[] = {20,80,50}, last_y[] = {80, 30, 100};
	uint8_t states[NUM_CHANNELS] = {0, 40, 80}, ds;
	
	while(1)
	{
		for (i = 0; i < NUM_ITENS; i++)
		{
			osMutexWait(context_mutex, osWaitForever);
			drawItem(itens_x[i], itens_y[i], last_x[i], last_y[i], states);
			osMutexRelease(context_mutex);
	
			last_x[i] = itens_x[i];
			last_y[i] = itens_y[i];
			itens_x[i] += dx[i];	
			if (itens_x[i] > 128 && dx[i] == 1)
				itens_x[i] = 0;
			if (itens_x[i] < 0 && dx[i] == -1)
				itens_x[i] = 128;
			
			for (j = 0; j < NUM_CHANNELS; j++)
			{
				if (states[i] > 120)
					ds = -2;
				if (states[i] < 2)
					ds = 2;
				
				states[i] += ds;
			}
		}
	}
}
