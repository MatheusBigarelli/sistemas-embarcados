#ifndef __MEM_POOL__
#define __MEM_POOL__

#include "util.h"
enum ID
{
    MainMenu,
		WaveformMenu,
    FreqMenu,
    AmpMenu,
    GanttMenu
}typedef ID;


typedef struct {
		ID currentMenu;
		bool updateMenu;
		bool parameterChanged;
		char buffer[16];
		uint8_t bufferIndex;
} UART_MEM_BLOCK_t; // Dados utilizados pela thread UART handler da interrupção da UART


void createMemPool();
#endif