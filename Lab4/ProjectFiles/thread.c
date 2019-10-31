#include "thread.h"

osThreadId tidUART;
osThreadDef(UART, osPriorityNormal, 1, 0);

osThreadId tidDisplay;
osThreadDef(Display, osPriorityNormal, 1, 0);

osThreadId tidSignalGenerator;
osThreadDef(SignalGenerator, osPriorityNormal, 1, 0);

osTimerId tidTimer;
osTimerDef(tidTimer, Timer);


void createThreads(void)
{
    osThreadCreate(osThread(UART), NULL);
    osThreadCreate(osThread(Display), NULL);
    osThreadCreate(osThread(SignalGenerator), NULL);
}

void createTimer(void)
{
    osTimerCreate(osTimer(tidTimer), osTimerPeriodic, NULL);
    osTimerStart(tidTimer, SIGGEN_TRIGGER_TIME);
}
