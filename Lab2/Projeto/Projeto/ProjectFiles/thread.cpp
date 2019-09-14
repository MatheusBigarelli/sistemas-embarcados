#include "thread.h"
#include "primes.h"
#include "messages.h"

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
	key = primes[current_iteration+1];
	key_ready = true;
}
void decodeMessageThread(void const* arg)
{
	if(key_ready)
	{
			for(int i = 0; i<MESSAGE_LENGTH;i++)
			{
				message[i] = messages[current_message][i];
			}
			decode_ready = true;
	}
	else
	{
		osThreadYield();
	}
	
}
void testNextToLastWordThread(void const* arg)
{
	if(decode_ready)
	{
			while(1){};
	}
	else
	{
		osThreadYield();
	}
	
}
void testLastWordThread(void const* arg)
{
	int word;
	if(decode_ready)
	{
			word = (message[MESSAGE_LENGTH - 1]<<24) + (message[MESSAGE_LENGTH - 2]<<16) +
							(message[MESSAGE_LENGTH - 3]<<8) + (message[MESSAGE_LENGTH - 4]);
	}
	else
	{
		osThreadYield();
	}
}
void printMessageThread(void const* arg)
{
}