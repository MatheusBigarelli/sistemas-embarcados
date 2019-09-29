#ifndef __DECODE__
#define __DECODE__

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "cmsis_os.h"

#include "primes.h"
#include "messages.h"
#include "uart.h"
#include "util.h"


typedef enum
{
	GENERATING,
	DECODING,
	TESTING1,
	TESTING2,
	PRINTING,
	FINISHED
} state;




void primesGenerator(void const* arg);
void decoder(void const* arg);
void test1Validator(void const* arg);
void test2Validator(void const* arg);
void printer(void const* arg);


int16_t getNextKeyIndex(int16_t iteration);
void decodeMessage(int32_t* message);
int8_t decodeWithKey(int32_t* message);
int16_t getNextWord(int32_t* message, int16_t hw_index);
int32_t decodeWord(int32_t word, int16_t key_index);
void writeDecodedToBuffer(int32_t decoded_word, uint16_t word_index);
void printResults(void);
uint8_t testKey(int32_t* message, int16_t key, uint8_t word);

extern void SysTick_Wait1ms(uint32_t delay);

//extern osStatus;
//extern osThreadId;

//extern osStatus osThreadTerminate(osThreadId thread_id);
//extern osThreadId osThreadGetId(void);
//extern osStatus osThreadYield(void);
//extern osStatus osDelay(uint32_t milisec);

#endif
