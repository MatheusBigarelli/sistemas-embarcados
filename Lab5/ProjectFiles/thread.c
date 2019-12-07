#include "thread.h"
#include "matematica.h"

void Display(const void *args);


osThreadDef(ThreadA, osPriorityIdle, 1, 0);
osThreadId tidThreadA;
osTimerDef(TimerA, callbackTimer);
osTimerId tidTimerA;

osThreadDef(ThreadB, osPriorityIdle, 1, 0);
osThreadId tidThreadB;
osTimerDef(TimerB, callbackTimer);
osTimerId tidTimerB;

osThreadDef(ThreadC, osPriorityIdle, 1, 0);
osThreadId tidThreadC;
osTimerDef(TimerC, callbackTimer);
osTimerId tidTimerC;

osThreadDef(ThreadD, osPriorityIdle, 1, 0);
osThreadId tidThreadD;
osTimerDef(TimerD, callbackTimer);
osTimerId tidTimerD;

osThreadDef(ThreadE, osPriorityIdle, 1, 0);
osThreadId tidThreadE;
osTimerDef(TimerE, callbackTimer);
osTimerId tidTimerE;

osThreadDef(ThreadF, osPriorityIdle, 1, 0);
osThreadId tidThreadF;
osTimerDef(TimerF, callbackTimer);
osTimerId tidTimerF;

osThreadDef(UART, osPriorityIdle, 1, 0);
osThreadId tidUART;

osThreadId tidMain;

osThreadDef(Display, osPriorityIdle, 1, 0);
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
    
//    osTimerStart(tidTimerA, 125);
//    osTimerStart(tidTimerB, 500);
//    osTimerStart(tidTimerC, 200);
//    osTimerStart(tidTimerD, 1000);
//    osTimerStart(tidTimerE, 166);
//    osTimerStart(tidTimerF, 100);
    
    osTimerStart(tidTimerA, 2);
    osTimerStart(tidTimerB, 3);
    osTimerStart(tidTimerC, 4);
    osTimerStart(tidTimerD, 6);
    osTimerStart(tidTimerE, 5);
    osTimerStart(tidTimerF, 1);
}

void createMailQueue()
{
    qidUartMailQueue = osMailCreate(osMailQ(uartMailQ), NULL);
}

void callbackTimer(const void* args)
{
    uint32_t signal = (uint32_t)args;
    volatile uint8_t threadIndex;
    threadIndex = signalToIndex(signal); // Agora sabe qual thread teve seu timer ativado.
    threadsInfo[threadIndex].currentState = READY; // Tarefa pronta para executar
}

void threadSwitch(osThreadId tid)
{
    // Aumentar a prioridade da thread escolhida.
    // Fazer isso já troca a thread para a que tiver
    // a maior prioridade.
    osThreadSetPriority(tid, osPriorityNormal);
    osThreadSetPriority(tidMain, osPriorityIdle);
}

void threadYield()
{
    // Diminuir prioridade da thread atual
    // Ao fazer isso a thread já é retirada caso exista
    // uma thread com prioridade maior
    osThreadSetPriority(osThreadGetId(), osPriorityIdle);
    osThreadSetPriority(tidMain, osPriorityNormal);
}


THREAD_INDEX signalToIndex(uint32_t signal)
{
    switch(signal)
    {
        case SIG_THREAD_A:
            return THREAD_A_INDEX;
        case SIG_THREAD_B:
            return THREAD_B_INDEX;
        case SIG_THREAD_C:
            return THREAD_C_INDEX;
        case SIG_THREAD_D:
            return THREAD_D_INDEX;
        case SIG_THREAD_E:
            return THREAD_E_INDEX;
        case SIG_THREAD_F:
            return THREAD_F_INDEX;
    }
}


void initThreadsInfo()
{
	int i;
	// O -1 eh para evitar o escalonador
    threadsInfo[THREAD_A_INDEX].durationInTicks = 0x0682;
    threadsInfo[THREAD_A_INDEX].staticPriority = 10;
    threadsInfo[THREAD_A_INDEX].charId = 'A';
    threadsInfo[THREAD_A_INDEX].id = tidThreadA;
    
    threadsInfo[THREAD_B_INDEX].durationInTicks = 0x49AB;
    threadsInfo[THREAD_B_INDEX].staticPriority = 0;
    threadsInfo[THREAD_B_INDEX].charId = 'B';
    threadsInfo[THREAD_B_INDEX].id = tidThreadB;
    
    threadsInfo[THREAD_C_INDEX].durationInTicks = 0x15F5;
    threadsInfo[THREAD_C_INDEX].staticPriority = -30;
    threadsInfo[THREAD_C_INDEX].charId = 'C';
    threadsInfo[THREAD_C_INDEX].id = tidThreadC;
    
    threadsInfo[THREAD_D_INDEX].durationInTicks = 0x0CF0;
    threadsInfo[THREAD_D_INDEX].staticPriority = 0;
    threadsInfo[THREAD_D_INDEX].charId = 'D';
    threadsInfo[THREAD_D_INDEX].id = tidThreadD;
    
    threadsInfo[THREAD_E_INDEX].durationInTicks = 0x1E29;
    threadsInfo[THREAD_E_INDEX].staticPriority = -30;
    threadsInfo[THREAD_E_INDEX].charId = 'E';
    threadsInfo[THREAD_E_INDEX].id = tidThreadE;
    
    threadsInfo[THREAD_F_INDEX].durationInTicks = 0x2829;
    threadsInfo[THREAD_F_INDEX].staticPriority = -100;
    threadsInfo[THREAD_F_INDEX].charId = 'F';
    threadsInfo[THREAD_F_INDEX].id = tidThreadF;
    
    
	for(i=0;i<TOTAL_THREADS - 1;i++){
		threadsInfo[i].currentState = READY;
        threadsInfo[i].delayInTicks = 0;
        //Na primeira iteracao o Prazo estimado = Prazo real, logo tempo de relaxamento eh zero
        threadsInfo[i].laxityTimeInTicks = 0;
        threadsInfo[i].executionPercent = 0;
        threadsInfo[i].tickOfDeadline = threadsInfo[i].durationInTicks;
        threadsInfo[i].dinamicPriority = threadsInfo[i].staticPriority;
	}
}
