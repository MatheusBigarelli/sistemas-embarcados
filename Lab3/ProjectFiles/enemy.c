//#include "enemy.h"


//int16_t ENEMY_Y(int16_t X) { return (FLOOR_BASE_PIXEL+21*X-ENEMY_HEIGHT); }

//extern int16_t enemies_x[], enemies_y[];
//extern osMutexId context_mutex;
//extern tContext sContext;

//const uint8_t enemy[] = {
//0x1c,0x24,0x38,0x40,0x54,0x7c,0x58,0x74,0xac,0x64,0x84,0xc8,0x64,0x84,0xc4,0x54,0x6c,0xa4,0x38,0x48,0x70,0x14,0x1c,0x28
//,0x60,0x80,0xc0,0x54,0x70,0xa8,0x2c,0x3c,0x58,0x64,0x80,0xc4,0x58,0x74,0xb0,0x28,0x38,0x58,0x5c,0x78,0xb8,0x58,0x74,0xb0
//,0x34,0x48,0x6c,0x4c,0x5c,0x80,0x7c,0x8c,0xa8,0x3c,0x50,0x74,0x48,0x58,0x7c,0x84,0x90,0xb0,0x58,0x68,0x90,0x4c,0x5c,0x80
//,0x10,0x10,0x10,0x48,0x48,0x48,0xb4,0xb4,0xb4,0x10,0x10,0x0c,0x24,0x24,0x24,0xc4,0xc4,0xc4,0xbc,0xbc,0xbc,0xb0,0xb0,0xac
//,0xc0,0xc0,0xc0,0xd0,0xd0,0xd0,0xbc,0xbc,0xbc,0x10,0x10,0x10,0x08,0x08,0x08,0x2c,0x2c,0x2c,0x34,0x34,0x34,0x30,0x30,0x30
//};


//uint32_t enemyOneChannel[ENEMY_PIXELS];
//void drawEnemy(int16_t x, int16_t y, int16_t last_x, int16_t last_y, bool feet_state)
//{
//    int i, j;
//	
//	j = 0;
//    for (i = 0; i < ENEMY_PIXELS; i++)
//    {
//        enemyOneChannel[j] = (enemy[3*i]<<16) + (enemy[3*i+1]<<8) + (enemy[3*i+2]);
//        j++;
//    }
//    
//    GrContextBackgroundSet(&sContext, ClrBlack);
//    
//	
//	GrContextForegroundSet(&sContext, ClrBlack);
//	
//	
//	if (last_x < x)
//	{
//		for (i = 0; i < ENEMY_HEIGHT; i++)
//		{
//			GrPixelDraw(&sContext, last_x, i+last_y);
//			GrPixelDraw(&sContext, last_x+1, i+last_y);
//		}
//	}
//	
//	else
//	{
//		for (i = 0; i < ENEMY_HEIGHT; i++)
//		{
//			GrPixelDraw(&sContext, last_x+ENEMY_WIDTH-1, i+last_y);
//			GrPixelDraw(&sContext, last_x+ENEMY_WIDTH-2, i+last_y);
//		}
//	}
//	
//    for (i = 0; i < ENEMY_HEIGHT; i++)
//    {
//        for (j = 0; j < ENEMY_WIDTH; j++)
//        {
//			if (feet_state == LEFT_FOOT_UP)
//				GrContextForegroundSet(&sContext, enemyOneChannel[i*ENEMY_WIDTH + j]);
//			else
//				GrContextForegroundSet(&sContext, enemyOneChannel[i*ENEMY_WIDTH + (ENEMY_WIDTH-1)-j]);
//            GrPixelDraw(&sContext,j+x,i+y);
//        }
//    }
//}


void Enemy(void const *args)
{
//	uint8_t i, j;
//	int16_t dx[] = {2, 2}, last_x[] = {20, 65}, last_y[] = {80, 45};
//	bool feet_states[] = {LEFT_FOOT_UP, RIGHT_FOOT_UP};
//	uint8_t feet_time = 0;
//	
//	while(1)
//	{
//		for (i = 0; i < 2; i++)
//		{
//			osMutexWait(context_mutex, osWaitForever);
//			drawEnemy(enemies_x[i], enemies_y[i], last_x[i], last_y[i], feet_states[i]);
//			osMutexRelease(context_mutex);
//		
//			last_x[i] = enemies_x[i];
//			last_y[i] = enemies_y[i];
//			enemies_x[i] += dx[i];
//			if (enemies_x[i] >= 128-ENEMY_WIDTH)
//				dx[i] *= -1;
//			if (enemies_x[i] <= 0)
//				dx[i] *= -1;
//			feet_time = (feet_time+1) % 10;
//			if (feet_time == 0)
//			{
//				for (j = 0; j < NUM_ENEMIES; j++)
//					feet_states[j] = !feet_states[j];
//			}
//		}
//	}
}
