#include "TM4C129.h"                    // Device header
#include <stdint.h>
#include "cmsis_os.h"
#include "thread.h"
#include "gpio.h"
#include "uart.h"

int main()
{
	//osKernelInitialize();
	//initThreads();
	GPIO_Init();
	UART_Init();
	UART0_TxChar('a');
	//osKernelStart();
	//osDelay(osWaitForever);
}