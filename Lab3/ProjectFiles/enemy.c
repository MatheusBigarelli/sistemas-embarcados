#include "enemy.h"

extern osMutexId context_mutex;
extern tContext sContext;

extern Enemy enemies[NUM_ENEMIES];


int16_t ENEMY_Y(int16_t X) { return (FLOOR_BASE_PIXEL+21*X-ENEMY_HEIGHT); }


const uint8_t enemy_img[] = {
0x1c,0x24,0x38,0x40,0x54,0x7c,0x58,0x74,0xac,0x64,0x84,0xc8,0x64,0x84,0xc4,0x54,0x6c,0xa4,0x38,0x48,0x70,0x14,0x1c,0x28
,0x60,0x80,0xc0,0x54,0x70,0xa8,0x2c,0x3c,0x58,0x64,0x80,0xc4,0x58,0x74,0xb0,0x28,0x38,0x58,0x5c,0x78,0xb8,0x58,0x74,0xb0
,0x34,0x48,0x6c,0x4c,0x5c,0x80,0x7c,0x8c,0xa8,0x3c,0x50,0x74,0x48,0x58,0x7c,0x84,0x90,0xb0,0x58,0x68,0x90,0x4c,0x5c,0x80
,0x10,0x10,0x10,0x48,0x48,0x48,0xb4,0xb4,0xb4,0x10,0x10,0x0c,0x24,0x24,0x24,0xc4,0xc4,0xc4,0xbc,0xbc,0xbc,0xb0,0xb0,0xac
,0xc0,0xc0,0xc0,0xd0,0xd0,0xd0,0xbc,0xbc,0xbc,0x10,0x10,0x10,0x08,0x08,0x08,0x2c,0x2c,0x2c,0x34,0x34,0x34,0x30,0x30,0x30
};


uint32_t enemyOneChannel[ENEMY_PIXELS];
void drawEnemy(Enemy enemy)
{
    int i, j;
	
	j = 0;
    for (i = 0; i < ENEMY_PIXELS; i++)
    {
        enemyOneChannel[j] = (enemy_img[3*i]<<16) + (enemy_img[3*i+1]<<8) + (enemy_img[3*i+2]);
        j++;
    }
    
    GrContextBackgroundSet(&sContext, ClrBlack);
    
	
	GrContextForegroundSet(&sContext, ClrBlack);
	
	
	if (enemy.last_x < enemy.x)
	{
		for (i = 0; i < ENEMY_HEIGHT; i++)
		{
			GrPixelDraw(&sContext, enemy.last_x, i+enemy.last_y);
			GrPixelDraw(&sContext, enemy.last_x+1, i+enemy.last_y);
		}
	}
	
	else
	{
		for (i = 0; i < ENEMY_HEIGHT; i++)
		{
			GrPixelDraw(&sContext, enemy.last_x+ENEMY_WIDTH-1, i+enemy.last_y);
			GrPixelDraw(&sContext, enemy.last_x+ENEMY_WIDTH-2, i+enemy.last_y);
		}
	}
	
    for (i = 0; i < ENEMY_HEIGHT; i++)
    {
        for (j = 0; j < ENEMY_WIDTH; j++)
        {
			if (enemy.feet_state == LEFT_FOOT_UP)
				GrContextForegroundSet(&sContext, enemyOneChannel[i*ENEMY_WIDTH + j]);
			else
				GrContextForegroundSet(&sContext, enemyOneChannel[i*ENEMY_WIDTH + (ENEMY_WIDTH-1)-j]);
            GrPixelDraw(&sContext,j+enemy.x,i+enemy.y);
        }
    }
}


void EnemyThread(void const *args)
{
	uint8_t i, j;
	int16_t dx[] = {2, 2}, last_x[] = {20, 65}, last_y[] = {80, 45};
	bool feet_states[] = {LEFT_FOOT_UP, RIGHT_FOOT_UP};
	uint8_t feet_time = 0;
	
	for (i = 0; i < NUM_ENEMIES; i++)
	{
		enemies[i].x = 30*i;
		enemies[i].y = ENEMY_Y(FLOOR(1+i));
		enemies[i].last_x = enemies[i].x;
		enemies[i].last_y = enemies[i].y;
		enemies[i].feet_state = LEFT_FOOT_UP;
		enemies[i].speed = 2;
	}
	
	while(1)
	{
		for (i = 0; i < NUM_ENEMIES; i++)
		{
			osMutexWait(context_mutex, osWaitForever);
			drawEnemy(enemies[i]);
			osMutexRelease(context_mutex);
		
			enemies[i].last_x = enemies[i].x;
			enemies[i].last_y = enemies[i].y;
			enemies[i].x += enemies[i].speed;
			if (enemies[i].x >= 128-ENEMY_WIDTH)
				enemies[i].speed *= -1;
			if (enemies[i].x <= 0)
				enemies[i].speed *= -1;
			feet_time = (feet_time+1) % 10;
			if (feet_time == 0)
			{
				for (j = 0; j < NUM_ENEMIES; j++)
					enemies[j].feet_state = !enemies[j].feet_state;
			}
		}
	}
}
