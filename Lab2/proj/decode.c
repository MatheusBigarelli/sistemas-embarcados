#include "decode.h"

uint8_t current_message_index = 0;
uint8_t buffer[4*MESSAGE_LENGTH];
uint16_t key_index;
uint8_t tests_ok = 0;

extern const int16_t primes[];
extern uint8_t current_state;
extern int32_t messages[NUMBER_OF_MESSAGES][MESSAGE_LENGTH];

void primesGenerator(void const* arg)
{
	for (int i = 0; i < NUMBER_OF_MESSAGES; i++)
	{	
		for (uint16_t key_ind = 0; key_ind < NUMBER_OF_PRIMES; key_ind++)
		{
			if (current_state == GENERATING)
			{
				key_index = key_ind;
				current_state = DECODING;
				osThreadYield();
			}
		}
	}
}


void decoder(void const* arg)
{
	if (current_state == DECODING)
	{
		int8_t buffer[4*MESSAGE_LENGTH];
		int32_t* message = messages[current_message_index];
		
		decodeMessage(message, buffer);
		
		current_state = TESTING1;
		osThreadYield();
	}
}


void test1Validator(void const* arg)
{
	if (current_state == TESTING1)
	{
		int32_t* message = messages[current_message_index];
		int16_t key = primes[key_index];
		
		// Testar a penúltima word
		uint8_t word = 1;
		int8_t test1_ok = testKey(message, key, word);
		
		if (test1_ok)
			tests_ok++;
		
		current_state = TESTING2;
		osThreadYield();
	}
}


void test2Validator(void const* arg)
{
	if (current_state == TESTING2)
	{
		int32_t* message = messages[current_message_index];
		int16_t key = primes[key_index];
		
		// Testar a última word
		uint8_t word = 0;		
		int8_t test2_ok = testKey(message, key, word);
		
		if (test2_ok)
			tests_ok += 2;
		
		current_state = PRINTING;
		osThreadYield();
	}
	
}





void printer(void const* arg)
{
	if (current_state == PRINTING)
	{
		printResults();
		
		if (current_message_index < NUMBER_OF_MESSAGES)
		{
			current_message_index++;
			current_state = GENERATING;
			osThreadYield();
		}
	}
}


void decodeMessage(int32_t* message, int8_t* buffer)
{
	int16_t iteration = 0;
	int8_t decrypted_ok = 0;
	
	while (iteration < NUMBER_OF_PRIMES)
	{
		int16_t key_index = getNextKeyIndex(iteration);
		decrypted_ok = decodeWithKey(message, buffer, key_index);
		printResults((int8_t*) buffer, primes[key_index]);
		if (decrypted_ok)
		{
			current_state = TESTING1;
			osThreadYield();
		}	
		iteration++;
	}
}



void printResults()
{
	uint8_t log[80];
	sprintf(log, "Key - %4d\r\n", primes[key_index]);
	if (tests_ok == 1)
		sprintf(log + 12, "Test 1 ok...\r\n");
	if (tests_ok == 2)
		sprintf(log + 12, "Test 2 ok...\r\n");
	if (tests_ok == 3)
	{
		sprintf(log + 12, "Test 1 ok...\r\n");
		sprintf(log + 26, "Test 2 ok...\r\n");
		sprintf(log + 40, "Key found!\r\n");
	}
	
	UART0_TxString(log);
	UART0_TxString((unsigned char*) buffer);
	UART0_TxString("\r\n\n");
}


int16_t getNextKeyIndex(int16_t iteration)
{
	// Busca linear
	int16_t key_index = iteration + 1;
	return key_index;
}

int8_t decodeWithKey(int32_t* message, int8_t* buffer, int16_t key_index)
{
	// Tiramos as duas últimas words de confirmação
	int16_t word_index;
	for (word_index = 0; word_index < (MESSAGE_LENGTH-2); word_index++)
	{
		int32_t word = message[word_index];
		int lixo = 0;
		int32_t decoded_word = decodeWord(word, key_index);
		
		// Gravando word decodificada no buffer
		int8_t chars[4];
		for (int i = 0; i < 4; i++)
		{
			chars[i] = (int8_t) (decoded_word >> 8*i);
			if (chars[i] < 0x30 || chars[i] > 0x7E)
			{
				chars[i] = 0x20;
			}
			buffer[4*word_index+i] = chars[i];
		}
	}
	
	int32_t last_conf_word = message[MESSAGE_LENGTH-1];
	int32_t sec_to_last_conf_word = message[MESSAGE_LENGTH-2];
	
	if (last_conf_word == primes[key_index]*primes[key_index-1] &&
			sec_to_last_conf_word == primes[key_index]*primes[key_index+1])
		return 1;
	
	return 0;
}

int32_t decodeWord(int32_t word, int16_t key_index)
{
	int32_t decoded_word = 0;
	int16_t temp;

	// Upper half
	temp = (int16_t) (word >> 16);
	temp /= (primes[key_index] - primes[key_index+1]);
	
	decoded_word |= (int32_t) (temp) << 16;
	
	// Lower half
	temp = (int16_t) word;
	temp /= (primes[key_index] - primes[key_index-1]);
	decoded_word |= ((int32_t) temp) & 0x0000FFFF;
	
	return decoded_word;
}

	
uint8_t testKey(int32_t* message, int16_t key, uint8_t word)
{
	uint8_t test_ok = 0;
	
	int32_t expected_product = message[MESSAGE_LENGTH-1 - word];
	int16_t factor;
	
	if (word == 1)
	{
		// Testando a penúltima word
		factor = primes[key_index+1];
	}
	else
	{
		// Testando a última word
		factor = primes[key_index-1];
	}
	int32_t calculated_product = key * factor;
	
	if (calculated_product == expected_product)
		test_ok = 1;
	
	return test_ok;
}
	
