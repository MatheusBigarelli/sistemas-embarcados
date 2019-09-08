#include "TM4C129.h"                    // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

#include "decode.h"

extern void decodeMessage(uint8_t* message, uint8_t* buffer);
extern uint8_t messages[NUMBER_OF_MESSAGES][MESSAGE_LENGTH];

int main()
{
	uint8_t cur_msg = 0;
	uint8_t buffer[MESSAGE_LENGTH];
	while (1)
	{
		decodeMessage(messages[cur_msg], (uint8_t*)buffer);
	}
	
	return 0;
}
