#include "thread.h"

void ThreadA(const void *args);
void ThreadB(const void *args);
void ThreadC(const void *args);
void ThreadD(const void *args);
void ThreadE(const void *args);
void ThreadF(const void *args);

osThreadDef(ThreadA, osPriorityNormal, 1, 0);
osThreadId tidThreadA;
osTimerDef(TimerA, callbackTimer);
osTimerId tidTimerA;

osThreadDef(ThreadB, osPriorityNormal, 1, 0);
osThreadId tidThreadB;
osTimerDef(TimerA, callbackTimer);
osTimerId tidTimerA;

osThreadDef(ThreadC, osPriorityNormal, 1, 0);
osThreadId tidThreadC;
osTimerDef(TimerA, callbackTimer);
osTimerId tidTimerA;

osThreadDef(ThreadD, osPriorityNormal, 1, 0);
osThreadId tidThreadD;
osTimerDef(TimerA, callbackTimer);
osTimerId tidTimerA;

osThreadDef(ThreadE, osPriorityNormal, 1, 0);
osThreadId tidThreadE;
osTimerDef(TimerA, callbackTimer);
osTimerId tidTimerA;

osThreadDef(ThreadF, osPriorityNormal, 1, 0);
osThreadId tidThreadF;
osTimerDef(TimerA, callbackTimer);
osTimerId tidTimerA;

osThreadId tidMain;


void createThreads()
{
    tidThreadA = osThreadCreate(osThread(ThreadA), NULL);
    tidThreadB = osThreadCreate(osThread(ThreadB), NULL);
    tidThreadC = osThreadCreate(osThread(ThreadC), NULL);
    tidThreadD = osThreadCreate(osThread(ThreadD), NULL);
    tidThreadE = osThreadCreate(osThread(ThreadE), NULL);
    tidThreadF = osThreadCreate(osThread(ThreadF), NULL);    
}

void createTimers()
{
    tidTimerA = osTimerCreate(osTimer(TimerA), osTimerPeriodic, (void*) SIG_THREAD_A);
    tidTimerA = osTimerCreate(osTimer(TimerA), osTimerPeriodic, (void*) SIG_THREAD_B);
    tidTimerA = osTimerCreate(osTimer(TimerA), osTimerPeriodic, (void*) SIG_THREAD_C);
    tidTimerA = osTimerCreate(osTimer(TimerA), osTimerPeriodic, (void*) SIG_THREAD_D);
    tidTimerA = osTimerCreate(osTimer(TimerA), osTimerPeriodic, (void*) SIG_THREAD_E);
    tidTimerA = osTimerCreate(osTimer(TimerA), osTimerPeriodic, (void*) SIG_THREAD_F);
}

void callbackTimer(const void* args)
{
    int32_t signal = args;

    osSignalSet(tidMain, signal);
}


void threadYield()
{

}

