#include "decode.h"

extern const uint16_t primes[];


void decodeMessage(uint8_t* message, uint8_t* buffer)
{
	uint16_t iteration = 0;
	while (iteration < NUMBER_OF_PRIMES)
	{
		uint16_t key_index = getNextKeyIndex(iteration);
		decodeWithKey(message, buffer, key_index);
		iteration++;
	}
}

uint16_t getNextKeyIndex(uint16_t iteration)
{
	// Busca linear
	uint16_t key_index = iteration + 1;
	return key_index;
}

void decodeWithKey(uint8_t* message, uint8_t* buffer, uint16_t key_index)
{
	// Vamos até metade do tamanho da mensagem,
	// porque ela está codificada em HALFWORD
	// Incrementamos o hw_index de 1 em 1 para poder diferenciar entre
	// halfwords de índice par e ímpar
	for (uint16_t hw_index = 0; hw_index < MESSAGE_LENGTH / 2; hw_index++)
	{
		uint16_t halfword = getNextHalfword(message, hw_index);
		uint16_t decoded_hw = decodeHalfword(halfword, hw_index, key_index);
		// Gravando halfword decodificada no buffer
		buffer[2*hw_index] = (uint8_t) decoded_hw;
		buffer[2*hw_index+1] = (uint8_t) (decoded_hw >> 8);
	}
}

uint16_t getNextHalfword(uint8_t* message, uint16_t hw_index)
{
	uint16_t halfword = (message[2*hw_index+1] << 8) + message[2*hw_index];
	return halfword;
}

uint16_t decodeHalfword(uint16_t halfword, uint16_t hw_index, uint16_t key_index)
{
	uint16_t decoded_hw = 0;
	// Se o índice for par
	if (!(hw_index % 2))
		decoded_hw = (uint8_t) ( halfword / (primes[key_index] - primes[key_index-1]) );
	
	else
		decoded_hw = (uint8_t) ( halfword / (primes[key_index] - primes[key_index+1]) );
	
	return decoded_hw;
}
	
	
