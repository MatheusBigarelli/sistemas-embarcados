#include "thread.h"
#include "matematica.h"

void Display(const void *args);
uint32_t ticksOffset;
uint16_t remaingThreadToCompleteCycle = TOTAL_MATH_THREADS;

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
    
    osTimerStart(tidTimerA, 125);
    osTimerStart(tidTimerB, 500);
    osTimerStart(tidTimerC, 200);
    osTimerStart(tidTimerD, 1000);
    osTimerStart(tidTimerE, 166);
    osTimerStart(tidTimerF, 100);
    
//    osTimerStart(tidTimerA, 2);
//    osTimerStart(tidTimerB, 3);
//    osTimerStart(tidTimerC, 4);
//    osTimerStart(tidTimerD, 6);
//    osTimerStart(tidTimerE, 5);
//    osTimerStart(tidTimerF, 1);
}

void callbackTimer(const void* args)
{
    uint32_t signal = (uint32_t)args;
    volatile uint8_t threadIndex;
    threadIndex = signalToIndex(signal); // Agora sabe qual thread teve seu timer ativado.
    threadsInfo[threadIndex].currentState = READY; // Tarefa pronta para executar
    threadsInfo[threadIndex].startTick = osKernelSysTick();
}

// Muda thread do nosso escalonador para a definida pelo pelo parametro tid
void threadSwitch(osThreadId tid)
{
    // Aumentar a prioridade da thread escolhida.
    // Fazer isso já troca a thread para a que tiver
    // a maior prioridade.
    osThreadSetPriority(tid, osPriorityNormal);
    osThreadSetPriority(tidMain, osPriorityIdle);
}

// Thread atual entrega o processador para o nosso escalonador
void threadYield()
{
    // Diminuir prioridade da thread atual
    // Ao fazer isso a thread já é retirada caso exista
    // uma thread com prioridade maior
    osThreadSetPriority(osThreadGetId(), osPriorityIdle);
    osThreadSetPriority(tidMain, osPriorityNormal);
}
extern void UART0_TxString(char *data);

// Chamado quando thread termina de executar 
// Essencialmente verifica o prazo da tarefa (para ver ocorrencia de master/secondary faults), 
// move a tarefa para estado de esperando e por fim entrega o processador(threadYield)
void moveThreadToWaiting(THREAD_INDEX tindex) 
{
    uint32_t endTick = osKernelSysTick() - 124; //124 ticks ate obter o valor, a tarefa terminou antes
    uint32_t deadline = threadsInfo[tindex].tickOfDeadline;
    threadsInfo[tindex].currentState = WAITING; // Thread ja terminou de executar, agora espera ate periodo chegar 
    remaingThreadToCompleteCycle--;
    //Verifica prazos
    if(threadsInfo[tindex].isRealtime)
    {
        if(endTick - ticksOffset >deadline)
        {
            //UART0_TxString("Master Fault\r\n");
        }
    }
    else
    {
        if(endTick - ticksOffset > deadline)
        {
            //UART0_TxString("Secondary Fault\r\n");
            //UART0_TxString("Aumentar prioridade\r\n");
        }
        if(endTick - ticksOffset < deadline /2)
        {
            //UART0_TxString("Secondary Fault\r\n");
            //UART0_TxString("Diminuir prioridade\r\n");
        }
    }
    
    threadYield();
}

// Dado um signal - recebido pelo funcao de callback do timer - obtemos o indice correspondente da thread
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

// Dado um osThreadId obtemos o indice correspondente no vetor de threads
THREAD_INDEX tidToIndex(osThreadId tid)
{
    int i;
    for(i = 0; i< TOTAL_THREADS; i++)
    {
        if(threadsInfo[i].id == tid)
        {
            return i;
        }
    }
    return -1;
}

// Iniciliaza um conjunto de de informacoes sobre uma thread especifica - indica pelo parametro tindex.
void initThreadInfo(THREAD_INDEX tindex, uint32_t durationInTicks, double deadlinePercent, int32_t staticPriority, char charId, osThreadId id)
{
    threadsInfo[tindex].durationInTicks = durationInTicks;
    threadsInfo[tindex].staticPriority = staticPriority;
    threadsInfo[tindex].charId = charId;
    threadsInfo[tindex].id = id;
}
// Inicializa as informacoes de todas as threads matematicas
void initThreadsInfo()
{
	int i;
	uint32_t startTick;
    
    initThreadInfo(THREAD_A_INDEX, 0x0682, 0.7, 10, 'A', tidThreadA);
    initThreadInfo(THREAD_B_INDEX, 0x49AB, 0.5, 0, 'B', tidThreadB);
    initThreadInfo(THREAD_C_INDEX, 0x15F5, 0.3, -30, 'C', tidThreadC);
    initThreadInfo(THREAD_D_INDEX, 0x0CF0, 0.5, 0, 'D', tidThreadD);
    initThreadInfo(THREAD_E_INDEX, 0x1E29, 0.3, -30, 'E', tidThreadE);
    initThreadInfo(THREAD_F_INDEX, 0x2829, 0.1, -100, 'F', tidThreadF);    

    
    startTick = osKernelSysTick();

	for(i=0;i<TOTAL_MATH_THREADS;i++){
		threadsInfo[i].currentState = READY;
        threadsInfo[i].delayInTicks = 0;
        //Na primeira iteracao o Prazo estimado = Prazo real, logo tempo de relaxamento eh zero
        threadsInfo[i].laxityTimeInTicks = 0;
        threadsInfo[i].executionPercent = 0;
        threadsInfo[i].tickOfDeadline = threadsInfo[i].durationInTicks * ( 1 + threadsInfo[i].deadlinePercent);
        threadsInfo[i].isRealtime = false;
        threadsInfo[i].startTick = startTick - 100; // Tira alguns ticks para compensar, pois a thread de fato não começou ainda
        
        ganttInfo[i].activations = 0; // Aproveita o loop 
	}
    threadsInfo[THREAD_F_INDEX].isRealtime = true;
    //Offset de ticks
}
