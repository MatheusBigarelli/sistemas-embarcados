#include "thread.h"
#include "matematica.h"

void Display(const void *args);


osThreadDef(ThreadA, osPriorityNormal, 1, 0);
osThreadId tidThreadA;
osTimerDef(TimerA, callbackTimer);
osTimerId tidTimerA;

osThreadDef(ThreadB, osPriorityNormal, 1, 0);
osThreadId tidThreadB;
osTimerDef(TimerB, callbackTimer);
osTimerId tidTimerB;

osThreadDef(ThreadC, osPriorityNormal, 1, 0);
osThreadId tidThreadC;
osTimerDef(TimerC, callbackTimer);
osTimerId tidTimerC;

osThreadDef(ThreadD, osPriorityNormal, 1, 0);
osThreadId tidThreadD;
osTimerDef(TimerD, callbackTimer);
osTimerId tidTimerD;

osThreadDef(ThreadE, osPriorityNormal, 1, 0);
osThreadId tidThreadE;
osTimerDef(TimerE, callbackTimer);
osTimerId tidTimerE;

osThreadDef(ThreadF, osPriorityNormal, 1, 0);
osThreadId tidThreadF;
osTimerDef(TimerF, callbackTimer);
osTimerId tidTimerF;

osThreadDef(UART, osPriorityNormal, 1, 0);
osThreadId tidUART;

osThreadId tidMain;

osThreadDef(Display, osPriorityNormal, 1, 0);
osThreadId tidDisplay;
osMailQDef(uartMailQ, 1, Gantt_Info);
osMailQId qidUartMailQueue;

void createThreads()
{
    tidThreadA = osThreadCreate(osThread(ThreadA), NULL);
    tidThreadB = osThreadCreate(osThread(ThreadB), NULL);
    tidThreadC = osThreadCreate(osThread(ThreadC), NULL);
    tidThreadD = osThreadCreate(osThread(ThreadD), NULL);
    tidThreadE = osThreadCreate(osThread(ThreadE), NULL);
    tidThreadF = osThreadCreate(osThread(ThreadF), NULL);
    
    #if SIMULADOR == 0
    tidUART = osThreadCreate(osThread(UART), NULL);
    tidDisplay = osThreadCreate(osThread(Display), NULL);
    #endif
}

void createTimers()
{
    tidTimerA = osTimerCreate(osTimer(TimerA), osTimerPeriodic, (void*) SIG_THREAD_A);
    tidTimerB = osTimerCreate(osTimer(TimerB), osTimerPeriodic, (void*) SIG_THREAD_B);
    tidTimerC = osTimerCreate(osTimer(TimerC), osTimerPeriodic, (void*) SIG_THREAD_C);
    tidTimerD = osTimerCreate(osTimer(TimerD), osTimerPeriodic, (void*) SIG_THREAD_D);
    tidTimerE = osTimerCreate(osTimer(TimerE), osTimerPeriodic, (void*) SIG_THREAD_E);
    tidTimerF = osTimerCreate(osTimer(TimerF), osTimerPeriodic, (void*) SIG_THREAD_F);
}

void createMailQueue()
{
    qidUartMailQueue = osMailCreate(osMailQ(uartMailQ), NULL);
}

void callbackTimer(const void* args)
{
    uint32_t signal = *(uint32_t*)args;

    osSignalSet(tidMain, signal);
}


void threadYield()
{
    // Diminuir prioridade da thread atual
    osThreadSetPriority(osThreadGetId(), osPriorityIdle);
    // Aumentar prioridade do escalonador
    osThreadSetPriority(tidMain, osPriorityNormal);
    
    osThreadYield();
}

