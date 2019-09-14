#include "TM4C129.h"                    // Device header
#include <stdint.h>
#include "cmsis_os.h"
#include "thread.h"


int main()
{
	osKernelInitialize();
	initThreads();
	osKernelStart();
	//osDelay(osWaitForever);
}