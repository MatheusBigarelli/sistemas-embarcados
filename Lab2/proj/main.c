#include "TM4C129.h"                    // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

#include "decode.h"
#include "uart.h"
#include "util.h"
#include "gpio.h"

uint8_t current_state;



osThreadId tid_primesGenerator;
osThreadDef(primesGenerator, osPriorityNormal, 1, 0);

osThreadId tid_decoder;
osThreadDef(decoder, osPriorityNormal, 1, 0);

osThreadId tid_test1Validator;
osThreadDef(test1Validator, osPriorityNormal, 1, 0);

osThreadId tid_test2Validator;
osThreadDef(test2Validator, osPriorityNormal, 1, 0);

osThreadId tid_printer;
osThreadDef(printer, osPriorityNormal, 1, 0);




void threadInit(void)
{
	osThreadCreate(osThread(primesGenerator), NULL);
	osThreadCreate(osThread(decoder), NULL);
	osThreadCreate(osThread(test1Validator), NULL);
	osThreadCreate(osThread(test2Validator), NULL);
	osThreadCreate(osThread(printer), NULL);
}



int main()
{
	osKernelInitialize();
	
	PLL_Init();
	SysTick_Init();
	GPIO_Init();	
	UART_Init();
	
	threadInit();
	
	current_state = GENERATING;
	
	osKernelStart();
	
	osDelay(osWaitForever);
	
	return 0;
}


