#include "eddie.h"

extern int16_t eddie_x, eddie_y;
extern osMutexId context_mutex;
extern tContext sContext;

extern Eddie eddie;
extern Ladder ladders[NUM_LADDERS];
extern Enemy enemies[NUM_ENEMIES];

const uint8_t eddie_img[] = {	
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
void drawEddie(Eddie eddie)
{
    int16_t i, j;
	j = 0;
    for (i = 0; i < EDDIE_PIXELS; i++)
    {
        eddieOneChannel[j] = (eddie_img[3*i]<<16) + (eddie_img[3*i+1]<<8) + (eddie_img[3*i+2]);
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);

	clearTrace(eddie);
	
	if (eddie.jump_state == ON_GROUND)
	{
		for (i = 0; i < EDDIE_HEIGHT; i++)
		{
			for (j = 0; j < EDDIE_WIDTH; j++)
			{
				// Espelha o Eddie.
				if (eddie.last_face_direction == DIR_LEFT)
					GrContextForegroundSet(&sContext, eddieOneChannel[i*EDDIE_WIDTH + (EDDIE_WIDTH-1)-j]);
				else
					GrContextForegroundSet(&sContext, eddieOneChannel[i*EDDIE_WIDTH + j]);
				GrPixelDraw(&sContext,j+eddie.x,i+eddie.y);
			}
		}
	}
	else if (eddie.jump_state == HIGH)
	{
		for (i = 0; i < EDDIE_HEIGHT; i++)
		{
			for (j = 0; j < EDDIE_WIDTH; j++)
			{
				// Espelha o Eddie.
				if (eddie.last_face_direction == DIR_LEFT)
					GrContextForegroundSet(&sContext, eddieOneChannel[i*EDDIE_WIDTH + (EDDIE_WIDTH-1)-j]);
				else
					GrContextForegroundSet(&sContext, eddieOneChannel[i*EDDIE_WIDTH + j]);
				GrPixelDraw(&sContext,j+eddie.x,i*4/5+eddie.y);
			}
		}
		
		// Print feet
		for (j = 0; j < EDDIE_WIDTH; j++)
		{
			// Espelha o Eddie.
			if (eddie.last_face_direction == DIR_LEFT)
				GrContextForegroundSet(&sContext, eddieOneChannel[(EDDIE_HEIGHT-1)*EDDIE_WIDTH + (EDDIE_WIDTH-1)-j]);
			else
				GrContextForegroundSet(&sContext, eddieOneChannel[(EDDIE_HEIGHT-1)*EDDIE_WIDTH + j]);
			GrPixelDraw(&sContext,j+eddie.x,i*4/5+eddie.y);
		}
	}
	else
	{
		for (i = 0; i < EDDIE_HEIGHT; i++)
		{
			for (j = 0; j < EDDIE_WIDTH; j++)
			{
				// Espelha o Eddie.
				if (eddie.last_face_direction == DIR_LEFT)
					GrContextForegroundSet(&sContext, eddieOneChannel[i*EDDIE_WIDTH + (EDDIE_WIDTH-1)-j]);
				else
					GrContextForegroundSet(&sContext, eddieOneChannel[i*EDDIE_WIDTH + j]);
				GrPixelDraw(&sContext,j+eddie.x,i*3/5+eddie.y);
			}
		}
		// Print feet
		for (j = 0; j < EDDIE_WIDTH; j++)
		{
			// Espelha o Eddie.
			if (eddie.last_face_direction == DIR_LEFT)
				GrContextForegroundSet(&sContext, eddieOneChannel[(EDDIE_HEIGHT-1)*EDDIE_WIDTH + (EDDIE_WIDTH-1)-j]);
			else
				GrContextForegroundSet(&sContext, eddieOneChannel[(EDDIE_HEIGHT-1)*EDDIE_WIDTH + j]);
			GrPixelDraw(&sContext,j+eddie.x,i*3/5+eddie.y);
		}

	}
}



void clearTrace(Eddie eddie)
{
	GrContextForegroundSet(&sContext, ClrBlack);	
	deleteXTrace(eddie);
	deleteYTrace(eddie);
}


void deleteXTrace(Eddie eddie)
{
	int16_t i;
	
	if (eddie.last_x < eddie.x)
	{
		for (i = 0; i < EDDIE_HEIGHT; i++)
		{
			GrPixelDraw(&sContext, eddie.last_x, i+eddie.last_y);
			GrPixelDraw(&sContext, eddie.last_x+1, i+eddie.last_y);
		}
	}
	
	else if (eddie.last_x > eddie.x)
	{
		for (i = 0; i < EDDIE_HEIGHT; i++)
		{
			GrPixelDraw(&sContext, (EDDIE_WIDTH-1)+eddie.last_x, i+eddie.last_y);
			GrPixelDraw(&sContext, (EDDIE_WIDTH-2)+eddie.last_x, i+eddie.last_y);
		}
	}
}


void deleteYTrace(Eddie eddie)
{
	int16_t i, j, trace_len;
	
	if (eddie.last_y < eddie.y)
	{
		for (i = 0; i < EDDIE_WIDTH; i++)
		{
			trace_len = EDDIE_HEIGHT * (1 - 1/5*(5-eddie.jump_state));
			for (j = 0; j < trace_len; j++)
				GrPixelDraw(&sContext, i+eddie.last_x, j+eddie.last_y);
		}
	}
	
	else //if (eddie.last_y > eddie.y)
	{
		for (i = 0; i < EDDIE_WIDTH; i++)
		{
			trace_len = EDDIE_HEIGHT * (1 - 1/5*(5-eddie.jump_state));
			for (j = 0; j < trace_len; j++)
				GrPixelDraw(&sContext, i+eddie.last_x, j+eddie.last_y);
		}
	}
}



void EddieThread(void const *args)
{
	int16_t x, y, roof, floor;
	int16_t jump_direction;
	uint8_t jump_state = ON_GROUND;
	bool jump, jumping;
	uint16_t air_time = INIT_AIR_TIME;
	
	
	
	Object obj_eddie, obj_ladder, obj_enemy;
	
	obj_eddie.width = EDDIE_WIDTH;
	obj_eddie.height = EDDIE_HEIGHT;
	
	obj_ladder.width = LADDER_WIDTH;
	obj_ladder.height = LADDER_HEIGHT;
	
	obj_enemy.width = ENEMY_WIDTH;
	obj_enemy.height = ENEMY_HEIGHT;
	
	
	eddie.x = EDDIE_BASE_X;
	eddie.y = EDDIE_BASE_Y;
	eddie.id = ID_EDDIE;
	
	while (1)
	{		
		obj_ladder.x = ladders[0].x;
		obj_ladder.y = ladders[0].y;
		
		obj_eddie.x = eddie.x;
		obj_eddie.y = eddie.y;
		
		if (eddie.jump_state == HIGH)
			obj_eddie.height = 4/5*EDDIE_HEIGHT;
		else if (eddie.jump_state == ROOF)
			obj_eddie.height = 3/5*EDDIE_HEIGHT;
		else
			obj_eddie.height = EDDIE_HEIGHT;

		obj_enemy.x = enemies[0].x;
		obj_enemy.y = enemies[0].y;
		if (objectCollidedWith(obj_eddie, obj_ladder))
		{
			eddie.x = EDDIE_BASE_X;
			osSignalSet(osThreadId(PainelDeInstrumentos), );
		}
		if (objectCollidedWith(obj_eddie, obj_enemy))
			eddie.x = EDDIE_BASE_X;
		
		if (eddie.x != eddie.last_x || eddie.y != eddie.last_y)
		{
			osMutexWait(context_mutex, osWaitForever);
			drawEddie(eddie);
			osMutexRelease(context_mutex);
		}
		
		x = joy_read_x();
		y = joy_read_y();
		jump = button_read_s2();
		x = x*200/0xFFF-100;
		y = y*200/0xFFF-100;
		
		eddie.dx = 0;
		if (x > 50)
			eddie.dx = EDDIE_SPEED;
		if (x < -50)
			eddie.dx = -EDDIE_SPEED;
		eddie.dy = 0;
		
		if (jump && !jumping)
		{
			jump_direction = eddie.dx;
			jumping = true;
			air_time = INIT_AIR_TIME;
			roof = eddie.y - FREE_SPACE;
			floor = eddie.y+EDDIE_HEIGHT;
		}
		if (jumping)
		{
			if (air_time >= INIT_AIR_TIME-(FLOOR_SIZE-EDDIE_HEIGHT))
			{
				eddie.jump_state = HIGH;
				eddie.dy = -EDDIE_JUMP_SPEED;
			}
			else if (EDDIE_HEIGHT < air_time && air_time < INIT_AIR_TIME-(FLOOR_SIZE-EDDIE_HEIGHT))
			{
				eddie.jump_state = ROOF;
				eddie.dy = 0;
			}
			else if (air_time <= EDDIE_HEIGHT)
			{
				eddie.jump_state = HIGH;
				eddie.dy = EDDIE_JUMP_SPEED;
			}
			
			if (eddie.y + eddie.dy < roof)
			{
				eddie.jump_state = ROOF;
				eddie.dy = 0;
			}
			// y grows down on display
			if (eddie.y+EDDIE_HEIGHT + eddie.dy > floor)
			{
				eddie.jump_state = ON_GROUND;
				eddie.dy = 0;
			}
			air_time--;
			eddie.dx = jump_direction;
		}
		if (air_time == 0 && jumping)
		{
			eddie.jump_state = ON_GROUND;
			jumping = false;
		}
		
		eddie.last_x = eddie.x;
		eddie.last_y = eddie.y;
		
		eddie.x += eddie.dx;
		eddie.y += eddie.dy;
		
		if (eddie.x + EDDIE_WIDTH >= 128)
			eddie.x = 128-EDDIE_WIDTH;
		
		if (eddie.x < 0)
			eddie.x = 0;
		
		if (eddie.dx == EDDIE_SPEED)
			eddie.last_face_direction = DIR_RIGHT;
		else if (eddie.dx == -EDDIE_SPEED)
			eddie.last_face_direction = DIR_LEFT;
	}
}
