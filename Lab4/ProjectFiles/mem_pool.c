#include "mem_pool.h"

osPoolDef(uartPool,1,UART_MEM_BLOCK_t);
osPoolId uartPool_id;

void createMemPool()
{
	uartPool_id = osPoolCreate(osPool(uartPool));
	
}