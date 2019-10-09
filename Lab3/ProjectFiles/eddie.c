#include "eddie.h"

extern int16_t eddie_x, eddie_y;
extern osMutexId context_mutex;
extern tContext sContext;

const uint8_t eddie[] = {
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
 

uint32_t eddieOneChannel[90];
void drawEddie(int16_t x, int16_t y, int16_t last_x, int16_t last_y, bool last_face_direction)
{
    int16_t i, j;
	int numberOfPixels;

	numberOfPixels = sizeof(eddie)/sizeof(unsigned char);

	
	j = 0;
    for (i = 0; i < numberOfPixels - 3; i+=3)
    {
        eddieOneChannel[j] = (eddie[i]<<16) + (eddie[i+1]<<8) + (eddie[i+2]);
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);

	clearTrace(x, y, last_x, last_y);

    for (i = 0; i < EDDIE_HEIGHT; i++)
    {
        for (j = 0; j < EDDIE_WIDTH; j++)
        {
			// Espelha o Eddie.
			if (last_face_direction == DIR_LEFT)
				GrContextForegroundSet(&sContext, eddieOneChannel[i*EDDIE_WIDTH + (EDDIE_WIDTH-1)-j]);
			else
				GrContextForegroundSet(&sContext, eddieOneChannel[i*EDDIE_WIDTH + j]);
            GrPixelDraw(&sContext,j+x,i+y);
        }
    }
}


void clearTrace(int16_t x, int16_t y, int16_t last_x, int16_t last_y)
{
	GrContextForegroundSet(&sContext, ClrBlack);	
	deleteXTrace(x, last_x, y, last_y);
	deleteYTrace(y, last_y, x, last_x);
}


void deleteXTrace(int16_t x, int16_t last_x, int16_t y, int16_t last_y)
{
	int16_t i;
	
	if (last_x < x)
	{
		for (i = 0; i < EDDIE_HEIGHT; i++)
		{
			GrPixelDraw(&sContext, last_x, i+last_y);
			GrPixelDraw(&sContext, last_x+1, i+last_y);
		}
	}
	
	else if (last_x > x)
	{
		for (i = 0; i < EDDIE_HEIGHT; i++)
		{
			GrPixelDraw(&sContext, 5+last_x, i+last_y);
			GrPixelDraw(&sContext, 4+last_x, i+last_y);
		}
	}
}


void deleteYTrace(int16_t y, int16_t last_y, int16_t x, int16_t last_x)
{
    int16_t i, j, trace_del_inc, trace_del_signal;	

	if (last_y < y)
	{
		trace_del_inc = 0;
		trace_del_signal = 1;
	}
	else if (last_y > y)
	{
		trace_del_inc = EDDIE_HEIGHT-1;
		trace_del_signal = -1;
	}
	for (i = 0; i < EDDIE_WIDTH; i++)
		for (j = 0; j < EDDIE_SPEED; j++)
			GrPixelDraw(&sContext, i+last_x, last_y+trace_del_inc+trace_del_signal*j);
}



void Eddie(void const *args)
{
	int16_t x, y, roof, floor;
	int16_t dx, dy, jump_direction;
	bool jump, jumping, last_face_direction = DIR_RIGHT;
	uint16_t air_time = INIT_AIR_TIME;
	int16_t last_x = 64, last_y = 64;
	while (1)
	{
		osMutexWait(context_mutex, osWaitForever);
		drawEddie(eddie_x, eddie_y, last_x, last_y, last_face_direction);
		osMutexRelease(context_mutex);


		x = joy_read_x();
		y = joy_read_y();
		jump = button_read_s2();
		x = x*200/0xFFF-100;
		y = y*200/0xFFF-100;
		
		dx = 0;
		if (x > 50)
			dx = EDDIE_SPEED;
		if (x < -50)
			dx = -EDDIE_SPEED;
		dy = 0;
		
		if (jump && !jumping)
		{
			jump_direction = dx;
			jumping = true;
			air_time = INIT_AIR_TIME;
			roof = eddie_y-3;
			floor = eddie_y;
		}
		if (jumping)
		{
			if (air_time > INIT_AIR_TIME * 3/4)
				dy = -EDDIE_JUMP_SPEED;
			else if (INIT_AIR_TIME/4 < air_time && air_time < INIT_AIR_TIME * 3/4)
				dy = 0;
			else if (air_time <= INIT_AIR_TIME / 4)
				dy = EDDIE_JUMP_SPEED;
			if (eddie_y + dy < roof)
				dy = 0;
			// y grows down on display
			if (eddie_y + dy > floor)
				dy = 0;
			air_time--;
			dx = jump_direction;
		}
		if (air_time == 0 && jumping)
			jumping = false;

		
		last_x = eddie_x;
		last_y = eddie_y;
		
		eddie_x += dx;
		eddie_y += dy;
		
		if (eddie_x + EDDIE_WIDTH >= 128)
			eddie_x = 128-EDDIE_WIDTH;
		
		if (eddie_x < 0)
			eddie_x = 0;
		
		if (dx == EDDIE_SPEED)
			last_face_direction = DIR_RIGHT;
		else if (dx == -EDDIE_SPEED)
			last_face_direction = DIR_LEFT;
	}
}
