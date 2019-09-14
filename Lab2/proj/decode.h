#ifndef __DECODE__
#define __DECODE__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
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
	PRINTING
} state;




void primesGenerator(void const* arg);
void decoder(void const* arg);
void test1Validator(void const* arg);
void test2Validator(void const* arg);
void printer(void const* arg);


int16_t getNextKeyIndex(int16_t iteration);
void decodeMessage(int32_t* message, int8_t* buffer);
int8_t decodeWithKey(int32_t* message, int8_t* buffer, int16_t key);
int16_t getNextWord(int32_t* message, int16_t hw_index);
int32_t decodeWord(int32_t word, int16_t key_index);
void printResults();


#endif
