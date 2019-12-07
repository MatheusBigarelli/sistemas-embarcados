#ifndef __THREAD_H__
#define __THREAD_H__

#include "util.h"

extern void UART(void const* args);


extern osThreadId tidThreadA;
extern osThreadId tidThreadB;
extern osThreadId tidThreadC;
extern osThreadId tidThreadD;
extern osThreadId tidThreadE;
extern osThreadId tidThreadF;
extern osThreadId tidMain;

extern osMailQId qidUartMailQueue;

#define SIG_THREAD_A 0x00000000
#define SIG_THREAD_B 0x00000001
#define SIG_THREAD_C 0x00000002
#define SIG_THREAD_D 0x00000004
#define SIG_THREAD_E 0x00000008
#define SIG_THREAD_F 0x00000010


#define SIG_THREAD_ALL (SIG_THREAD_A | \
                         SIG_THREAD_B | \
                         SIG_THREAD_C | \
                         SIG_THREAD_D | \
                         SIG_THREAD_E | \
                         SIG_THREAD_F)

extern void callbackTimer(const void* args);
extern void createThreads();
extern void createTimers();
extern void createMailQueue();


void threadSwitch(osThreadId tid);
extern void threadYield();


#endif
