#include "item.h"

extern osMutexId context_mutex;
extern tContext sContext;

extern Item itens[NUM_ITENS];


// Cannot use macro with rand() in initialization.
int16_t ITEM_Y(int16_t X) { return (FLOOR_BASE_PIXEL+21*(X-1)+FLOOR_HEIGHT+1); }

const uint8_t item_img[] = {
 0x58,0x34,0x34,0xb4,0x6c,0x6c,0xcc,0x7c,0x7c,0x84,0x50,0x50,0xb4,0x6c,0x6c,0xcc,0x7c,0x7c,0x84,0x50,0x50
,0xb8,0x70,0x70,0xe0,0x88,0x88,0xe0,0x88,0x88,0xdc,0x88,0x88,0xe0,0x88,0x88,0xe0,0x88,0x88,0xdc,0x84,0x84
,0x40,0x28,0x28,0x94,0x58,0x58,0xd4,0x80,0x80,0xe0,0x88,0x88,0xdc,0x84,0x84,0xb4,0x6c,0x6c,0x64,0x3c,0x3c
,0x00,0x00,0x00,0x0c,0x08,0x08,0x58,0x34,0x34,0xb8,0x70,0x70,0x84,0x50,0x50,0x20,0x14,0x14,0x00,0x00,0x00
};	


uint32_t itemOneChannel[ITEM_PIXELS];
void drawItem(Item item)
{
    uint16_t i, j;
	uint32_t r, g, b;
	uint32_t foreground;
	
	j = 0;
    for (i = 0; i < ITEM_PIXELS; i++)
    {
		r = item_img[3*i];
		g = item_img[3*i+1];
		b = item_img[3*i+2];
		if (r > 10)
			r = (r+item.glow_state[0]) % 256;
		if (r < 10)
			r = item_img[i];
		
		if (g > 10)
			g = (g+item.glow_state[1]) % 256;
		if (g < 10)
			g = item_img[i];
		
		if (b > 10)
			b = (b+item.glow_state[2]) % 256;
		if (b < 10)
			b = item_img[i];
		
		itemOneChannel[j] = (r << 16) + (g << 8) + b;
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);
    
	
	GrContextForegroundSet(&sContext, ClrBlack);
	
	if (item.last_x < item.x)
	{
		for (i = 0; i < ITEM_HEIGHT; i++)
		{
			GrPixelDraw(&sContext,(item.last_x)%128,(i+item.last_y)%128);
		}
	}
	
	else
	{
		for (i = 0; i < ITEM_HEIGHT; i++)
		{
			GrPixelDraw(&sContext,((ITEM_WIDTH-1)+item.last_x)%128,(i+item.last_y)%128);
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
				GrPixelDraw(&sContext,(j+item.x)%128,(i+item.y)%128);
			}
        }
    }
}


void ItemThread(void const *args)
{
	uint8_t i, j;
	int16_t ds;
	for (i = 0; i < NUM_ITENS; i++)
	{
		itens[i].x = rand() % (128-ITEM_WIDTH);
		itens[i].y = ITEM_Y(FLOOR(i+2));
		itens[i].speed = rand()%3 - 1;
		itens[i].last_x = itens[i].x;
		itens[i].last_y = itens[i].y;
		itens[i].glow_state[0] = 0;
		itens[i].glow_state[1] = 40;
		itens[i].glow_state[2] = 80;
	}
	
	while(1)
	{
		for (i = 0; i < NUM_ITENS; i++)
		{
			osMutexWait(context_mutex, osWaitForever);
			drawItem(itens[i]);
			osMutexRelease(context_mutex);
	
			itens[i].last_x = itens[i].x;
			itens[i].last_y = itens[i].y;
			
			itens[i].x += itens[i].speed;	
			if (itens[i].x > 128 && itens[i].speed == 1)
				itens[i].x = 0;
			if (itens[i].x < 0 && itens[i].speed == -1)
				itens[i].x = 128;
			
			for (j = 0; j < NUM_CHANNELS; j++)
			{
				if (itens[i].glow_state[j] > 120)
					ds = -2;
				if (itens[i].glow_state[j] < 2)
					ds = 2;
				
				itens[i].glow_state[j] += ds;
			}
		}
	}
}

