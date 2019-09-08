#ifndef __DECODE__
#define __DECODE__

#include <string.h>
#include <stdint.h>
#include "primes.h"
#include "messages.h"

uint16_t getNextKeyIndex(uint16_t iteration);
void decodeMessage(uint8_t* message, uint8_t* buffer);
void decodeWithKey(uint8_t* message, uint8_t* buffer, uint16_t key);
uint16_t getNextHalfword(uint8_t* message, uint16_t hw_index);
uint16_t decodeHalfword(uint16_t halfword, uint16_t hw_index, uint16_t key_index);

#endif
