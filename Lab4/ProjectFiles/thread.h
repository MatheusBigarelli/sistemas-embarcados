#ifndef __THREAD__
#define __THREAD__

#include "util.h"

extern void UART(const void *args);
extern void Display(const void *args);
extern void SignalGenerator(const void *args);

extern osThreadId tidSignalGenerator;

extern void createThreads(void);
extern void createMailQueue(void);

extern osMessageQId qidUARTMsgBox;
extern osMailQId qidDisplayMailQueue;
extern osMailQId qidSigGenMailQueue;

#endif