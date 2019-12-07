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

void moveThreadToWaiting(THREAD_INDEX tindex) // Chamado quando thread termina de executar 
{
    threadsInfo[tindex].currentState = WAITING; // Thread ja terminou de executar, agora espera ate periodo chegar 
    threadsInfo[tindex].dinamicPriority = threadsInfo[tindex].staticPriority; //Reseta prioridade dinamica para estatica
    threadYield();
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


void initThreadInfo(THREAD_INDEX tindex, uint32_t durationInTicks, int32_t staticPriority, char charId, osThreadId id)
{
    threadsInfo[tindex].durationInTicks = durationInTicks;
    threadsInfo[tindex].staticPriority = staticPriority;
    threadsInfo[tindex].charId = charId;
    threadsInfo[tindex].id = id;
}

void initThreadsInfo()
{
	int i;
	// O -1 eh para evitar o escalonador
    initThreadInfo(THREAD_A_INDEX, 0x0682, 10, 'A', tidThreadA);
    initThreadInfo(THREAD_B_INDEX, 0x49AB, 0, 'B', tidThreadB);
    initThreadInfo(THREAD_C_INDEX, 0x15F5, -30, 'C', tidThreadC);
    initThreadInfo(THREAD_D_INDEX, 0x0CF0, 0, 'D', tidThreadD);
    initThreadInfo(THREAD_E_INDEX, 0x1E29, -30, 'E', tidThreadE);
    initThreadInfo(THREAD_F_INDEX, 0x2829, -100, 'F', tidThreadF);    
    
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
