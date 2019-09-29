#include "thread.h"
#include "primes.h"
#include "messages.h"
#include "gpio.h"
#include "uart.h"


int current_iteration = 0;
uint16_t key;
bool key_ready = false;
bool decode_ready = false;
bool testOK = false;
uint8_t message[MESSAGE_LENGTH] = {};

void initThreads(void)
{
	osThreadCreate(osThread(getKeyThread), NULL);
	osThreadCreate(osThread(decodeMessageThread), NULL);
	osThreadCreate(osThread(testNextToLastWordThread), NULL);
	osThreadCreate(osThread(testLastWordThread), NULL);
	osThreadCreate(osThread(printMessageThread), NULL);
}

void getKeyThread(void const* arg)
{
	while(1)
	{
		PortN_Output(1);
	}
}
void decodeMessageThread(void const* arg)
{
	while(1)
	{
		PortN_Output(0);
	}
}
void testNextToLastWordThread(void const* arg)
{	
	while(1)
	{
		if(current_iteration < 3)
		{
			UART0_TxString("Mensagem decodificada:\r\n");
			current_iteration++;
		}
		
		
	}
}
void testLastWordThread(void const* arg)
{
	while(1);
}
void printMessageThread(void const* arg)
{
	while(1);
}