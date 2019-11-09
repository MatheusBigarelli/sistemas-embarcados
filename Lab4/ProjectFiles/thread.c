#include "thread.h"

osThreadDef(UART, osPriorityNormal, 1, 0);
osThreadId tidUART;

osThreadDef(Display, osPriorityNormal, 1, 0);
osThreadId tidDisplay;

osThreadDef(SignalGenerator, osPriorityNormal, 1, 0);
osThreadId tidSignalGenerator;

osTimerDef(Timer, timerCallback);
osTimerId tidTimer;


void createThreads(void)
{
	tidUART = osThreadCreate(osThread(UART), NULL);
	#if SIMULADOR == 0
    
    tidSignalGenerator = osThreadCreate(osThread(SignalGenerator), NULL);
    //osThreadCreate(osThread(Display), NULL);
	#endif
}

void createTimer(void)
{
    tidTimer = osTimerCreate(osTimer(Timer), osTimerPeriodic, NULL);
    osTimerStart(tidTimer, SIGGEN_TRIGGER_TIME);
}
