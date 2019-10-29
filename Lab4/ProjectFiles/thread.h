#ifndef __THREAD__
#define __THREAD__

#include "util.h"

extern void UART(void const *args);
extern void Display(void const *args);
extern void SignalGenerator(void const *args);

extern void Timer(void const* args);

extern void createThreads(void);

#endif