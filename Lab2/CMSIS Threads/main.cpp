#include <stdint.h>
#include "cmsis_os.h"

//#define NUMBER_OF_TRIES 10000

//uint16_t generated_keys[NUMBER_OF_TRIES]; 
// 
//void initKeyGenerator(void);
//void generateKey(void const* arg);
//osThreadId tid_generateKey;
//osThreadDef(generateKey, osPriorityHigh, 1, 0);

void initThread(void);
void thread1(void const* arg);
osThreadId tid_thread1;
osThreadDef(thread1, osPriorityNormal, 1, 0);


void thread2(void const* arg);
osThreadId tid_thread2;
osThreadDef(thread2, osPriorityNormal, 1, 0);


int x = 0;

int main()
{
	osKernelInitialize();
	initThread();
	osKernelStart();
	
	return 0;
}

void initThread(void)
{
	osThreadCreate(osThread(thread1), NULL);
	osThreadCreate(osThread(thread2), NULL);
}

void thread1(void const* arg)
{
	while (1)
	{		
		x++;
		osDelay(2000);
	}
}


void thread2(void const* arg)
{
	while (1)
	{		
		x %= 4;
		osDelay(2000);
	}
}