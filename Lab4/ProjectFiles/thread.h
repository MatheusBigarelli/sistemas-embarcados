#ifndef __THREAD__
#define __THREAD__

#include "util.h"

extern void UART(const void *args);
extern void Display(const void *args);
extern void SignalGenerator(const void *args);
extern void timerCallback(const void *args);

extern osThreadId tidSignalGenerator;

extern void createThreads(void);
extern void createTimer(void);
extern void createMailQueue(void);

typedef enum
{
	SINUSOIDAL,
	TRIANGULAR,
	SAWTOOTH,
	SQUARE,
	TRAPEZOIDAL
}WAVEFORMS;

typedef struct
{
	double amplitude;
	double frequency;
	WAVEFORMS waveform;	
}signalConfig_t;


extern osMessageQId qidUARTMsgBox;

#endif