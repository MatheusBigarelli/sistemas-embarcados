#ifndef __THREAD__
#define __THREAD__

#include "util.h"

extern void UART(const void *args);
extern void Display(const void *args);
extern void SignalGenerator(const void *args);

extern osThreadId tidSignalGenerator;


extern void tickCounter(uint32_t* previousTick, char* name);

extern GanttInfo_t activationSeq[MAX_ACTIVATIONS];
extern uint32_t currentActivation;
extern bool activationSequenceFull;

extern void createThreads(void);
extern void createMailQueue(void);

extern osMessageQId qidUARTMsgBox;
extern osMailQId qidDisplayMailQueue;
extern osMailQId qidSigGenMailQueue;

#endif