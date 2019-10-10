#include "eddie.h"

extern int16_t eddie_x, eddie_y;
extern osMutexId context_mutex;
extern tContext sContext;

const uint8_t eddie[] = {	
 0x04,0x04,0x08,0x3c,0x44,0x5c,0x8c,0x9c,0xc0,0xac,0xc0,0xe8,0xac,0xc0,0xe8,0x8c,0x9c,0xc0,0x38,0x40,0x5c,0x04,0x04,0x08
,0x00,0x00,0x00,0x14,0x18,0x1c,0x9c,0xa0,0x58,0xe4,0xe4,0x74,0xe0,0xe4,0x70,0xb8,0xbc,0x64,0x54,0x54,0x34,0x08,0x08,0x04
,0x04,0x08,0x04,0x18,0x2c,0x18,0x84,0x9c,0x44,0xb8,0xcc,0x58,0xb8,0xcc,0x58,0x94,0xa8,0x48,0x3c,0x50,0x24,0x08,0x0c,0x04
,0x40,0x6c,0x40,0x54,0x90,0x54,0x5c,0x9c,0x5c,0x38,0x5c,0x34,0x18,0x28,0x18,0x2c,0x50,0x2c,0x50,0x8c,0x50,0x40,0x6c,0x40
,0x58,0x94,0x58,0x5c,0x9c,0x5c,0x5c,0x9c,0x5c,0x28,0x40,0x28,0x00,0x00,0x00,0x1c,0x30,0x1c,0x58,0x98,0x58,0x58,0x94,0x58
,0x54,0x94,0x54,0x5c,0x9c,0x5c,0x5c,0x9c,0x5c,0x28,0x44,0x28,0x00,0x00,0x00,0x1c,0x30,0x1c,0x58,0x98,0x58,0x58,0x94,0x58
,0x54,0x94,0x54,0x5c,0x9c,0x5c,0x5c,0x9c,0x5c,0x2c,0x48,0x2c,0x00,0x00,0x00,0x0c,0x18,0x0c,0x30,0x50,0x30,0x30,0x50,0x30
,0x54,0x90,0x54,0x5c,0x9c,0x5c,0x58,0x9c,0x5c,0x50,0x88,0x50,0x34,0x5c,0x34,0x1c,0x34,0x20,0x20,0x34,0x20,0x1c,0x2c,0x1c
,0x1c,0x2c,0x1c,0x3c,0x64,0x38,0x68,0x98,0x50,0x7c,0xac,0x58,0x78,0xa8,0x58,0x68,0x94,0x50,0x38,0x60,0x38,0x18,0x2c,0x18
,0x00,0x00,0x00,0x08,0x08,0x04,0x88,0x8c,0x38,0xcc,0xd0,0x50,0xcc,0xd0,0x50,0x88,0x8c,0x34,0x04,0x08,0x04,0x00,0x00,0x00
,0x00,0x00,0x00,0x04,0x04,0x00,0x8c,0x8c,0x34,0xd0,0xd0,0x50,0xd0,0xd0,0x50,0x94,0x94,0x38,0x1c,0x1c,0x0c,0x18,0x18,0x08
,0x00,0x00,0x00,0x04,0x04,0x00,0x8c,0x8c,0x34,0xd0,0xd0,0x50,0xd0,0xd0,0x50,0xc4,0xc4,0x4c,0xb0,0xb0,0x44,0xa8,0xa8,0x40
};


uint32_t eddieOneChannel[EDDIE_PIXELS];
void drawEddie(int16_t x, int16_t y, int16_t last_x, int16_t last_y, bool last_face_direction, uint8_t jump_state)
{
    int16_t i, j;

	
	j = 0;
    for (i = 0; i < EDDIE_PIXELS; i++)
    {
        eddieOneChannel[j] = (eddie[3*i]<<16) + (eddie[3*i+1]<<8) + (eddie[3*i+2]);
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);

	clearTrace(x, y, last_x, last_y, jump_state);
	
	if (jump_state == ON_GROUND)
	{
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
	else if (jump_state == HIGH)
	{
		for (i = 0; i < EDDIE_HEIGHT*3/4; i++)
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
	else
	{
		for (i = 0; i < EDDIE_HEIGHT*2/4; i++)
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
}



//void resizeImage3(int h1, int w1, int h2, int w2)
//{
//		int i, j;
//		int x_ratio = (int)((w1<<16)/w2) +1;
//    int y_ratio = (int)((h1<<16)/h2) +1;
//    int x2, y2 ;
//	
//    for (i=0;i<h2;i++)
//		{
//        for (j=0;j<w2;j++)
//				{
//            x2 = ((j*x_ratio)>>16) ;
//            y2 = ((i*y_ratio)>>16) ;
//            image.data[(i*w2)+j] = images[current_image][(y2*w1)+x2] ;
//        }                
//    }
//}


void clearTrace(int16_t x, int16_t y, int16_t last_x, int16_t last_y, uint8_t jump_state)
{
	GrContextForegroundSet(&sContext, ClrBlack);	
	deleteXTrace(x, last_x, y, last_y);
	deleteYTrace(y, last_y, x, last_x, jump_state);
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
			GrPixelDraw(&sContext, (EDDIE_WIDTH-1)+last_x, i+last_y);
			GrPixelDraw(&sContext, (EDDIE_WIDTH-2)+last_x, i+last_y);
		}
	}
}


void deleteYTrace(int16_t y, int16_t last_y, int16_t x, int16_t last_x, uint8_t jump_state)
{
    int16_t i, j, trace_del_off, trace_del_signal;	

	if (last_y < y)
	{
		trace_del_off = 0;
		trace_del_signal = 1;
	}
	else if (last_y > y)
	{
		trace_del_off = EDDIE_HEIGHT-1;
		trace_del_signal = -1;
	}

	if (jump_state == HIGH)
	{
		for (i = 0; i < EDDIE_WIDTH; i++)
			for (j = 0; j < EDDIE_JUMP_SPEED+EDDIE_HEIGHT/4; j++)
				GrPixelDraw(&sContext, i+last_x, last_y+trace_del_off+trace_del_signal*j);
	}
}



void Eddie(void const *args)
{
	int16_t x, y, roof, floor;
	int16_t dx, dy, jump_direction;
	uint8_t jump_state = ON_GROUND;
	bool jump, jumping, last_face_direction = DIR_RIGHT;
	uint16_t air_time = INIT_AIR_TIME;
	int16_t last_x = 64, last_y = 64;
	while (1)
	{
		osMutexWait(context_mutex, osWaitForever);
		drawEddie(eddie_x, eddie_y, last_x, last_y, last_face_direction, jump_state);
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
			roof = eddie_y - FREE_SPACE;
			floor = eddie_y+EDDIE_HEIGHT;
		}
		if (jumping)
		{
			if (air_time > INIT_AIR_TIME * 6/9)
			{
				jump_state = HIGH;
				dy = -EDDIE_JUMP_SPEED;
			}
			else if (INIT_AIR_TIME*3/9 < air_time && air_time < INIT_AIR_TIME * 7/4)
				dy = 0;
			else if (air_time <= INIT_AIR_TIME * 3/9)
			{
				jump_state = ROOF;
				dy = EDDIE_JUMP_SPEED;
			}
			
			if (eddie_y + dy < roof)
			{
				jump_state = ROOF;
				dy = 0;
			}
			// y grows down on display
			if (eddie_y+EDDIE_HEIGHT + dy > floor)
				dy = 0;
			
			air_time--;
			dx = jump_direction;
		}
		if (air_time == 0 && jumping)
		{
			jump_state = ON_GROUND;
			jumping = false;
		}
		
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
