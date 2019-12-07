/*============================================================================
 *                    Exemplos de utiliza��o do Kit
 *              EK-TM4C1294XL + Educational BooterPack MKII 
 *---------------------------------------------------------------------------*
 *                    Prof. Andr� Schneider de Oliveira
 *            Universidade Tecnol�gica Federal do Paran� (UTFPR)
 *===========================================================================
 * Autores das bibliotecas:
 * 		Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 * 		Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 * 		Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 * 		Mariana Carri�o        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/
#include "thread.h"
#include "uart.h"

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/
#include "grlib/grlib.h"
#include "cfaf128x128x16.h"


#define NUM_THREADS 7

//To print on the screen
tContext sContext;


ThreadInfo threadQueue[NUM_THREADS];


void initializeThreadInfos();
void startTask(osThreadId tid);
void schedule();
void updateThreadsInfos();
void sortThreadQueue();
void quick_sort(ThreadInfo *a, int left, int right);
void updatePriorities();


/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	cfaf128x128x16Init();
	GPIO_Init();
	UART_init();
}

void init_sidelong_menu(){
	uint8_t i;
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
}





/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void)
{
	Gantt_Info* info;
	
	osEvent event;

	uint8_t i;
	uint32_t startTick, finishTick;

	osKernelInitialize();

	#if SIMULADOR == 0
	init_all();
	init_sidelong_menu();
	#endif

	createThreads();
	createTimers();
	createMailQueue();
	
	osKernelStart();

	tidMain = osThreadGetId();
	clearUART();

	initializeThreadInfos();

	for (i = 0; i < NUM_THREADS; i++)
		startTask(threadQueue[i].tid);

	while (true)
	{
		schedule();
		startTick = osKernelSysTick();
		threadSwitch(threadQueue[0].tid);
		finishTick = osKernelSysTick();
		// Sempre a cabeça da fila é a thread a ser executada.
		threadQueue[0].totalTicks += finishTick - startTick;

		// Manda informações para onde precisa.
		// HINT: send info display uart.
		//mailMan();
		event = osSignalWait(SIG_GANTT, 0);
		if(event.status == osEventSignal){
			// Recebeu SIG_GANTT
			// Envia as informações necessárias para thread UART montar Gantt
			info = osMailAlloc(qidUartMailQueue, osWaitForever);
			info->ids[0] = 0;
			info->durations[0] = 100;
			osMailPut(qidUartMailQueue, info);
		}
	}

	return 0;
}

void initializeThreadInfos()
{
	uint8_t i;

	threadQueue[0].tid = tidThreadA;
	threadQueue[1].tid = tidThreadB;
	threadQueue[2].tid = tidThreadC;
	threadQueue[3].tid = tidThreadD;
	threadQueue[4].tid = tidThreadE;
	threadQueue[5].tid = tidThreadF;
	threadQueue[6].tid = tidDisplay;

	threadQueue[0].numberOfTicksNeeded = 0x682;
	threadQueue[1].numberOfTicksNeeded = 0x49AB;
	threadQueue[2].numberOfTicksNeeded = 0x15F5;
	threadQueue[3].numberOfTicksNeeded = 0xCF0;
	threadQueue[4].numberOfTicksNeeded = 0x1E29;
	threadQueue[5].numberOfTicksNeeded = 0x2829;
	threadQueue[6].numberOfTicksNeeded = 0x10000; // Display
	
	threadQueue[0].duePercentage = 1.70;
	threadQueue[1].duePercentage = 1.50;
	threadQueue[2].duePercentage = 1.30;
	threadQueue[3].duePercentage = 1.50;
	threadQueue[4].duePercentage = 1.30;
	threadQueue[5].duePercentage = 1.10;
	threadQueue[6].duePercentage = 100; // Display

	threadQueue[0].state = READY;
	threadQueue[1].state = READY;
	threadQueue[2].state = READY;
	threadQueue[3].state = READY;
	threadQueue[4].state = READY;
	threadQueue[5].state = READY;
	threadQueue[6].state = WAITING;

	threadQueue[0].staticPrio = 10; // LOW
	threadQueue[1].staticPrio = 0; // NORMAL
	threadQueue[2].staticPrio = -30; // HIGH
	threadQueue[3].staticPrio = 0; // NORMAL
	threadQueue[4].staticPrio = -30; // HIGH
	threadQueue[5].staticPrio = -100; // REAL-TIME
	threadQueue[6].staticPrio = 1000; // DISPLAY

	threadQueue[0].dynamicPrio = threadQueue[0].staticPrio;
	threadQueue[1].dynamicPrio = threadQueue[1].staticPrio;
	threadQueue[2].dynamicPrio = threadQueue[2].staticPrio;
	threadQueue[3].dynamicPrio = threadQueue[3].staticPrio;
	threadQueue[4].dynamicPrio = threadQueue[4].staticPrio;
	threadQueue[5].dynamicPrio = threadQueue[5].staticPrio;
	threadQueue[6].dynamicPrio = threadQueue[6].staticPrio;

	for (i = 0; i < NUM_THREADS; i++)
		startTask(threadQueue[i].tid);
}


void startTask(osThreadId tid)
{
	uint8_t i;
	for (i = 0; i < NUM_THREADS; i++)
		if (threadQueue[i].tid == tid)
			break;
	
	threadQueue[i].totalTicks = 0;
	threadQueue[i].startTick = osKernelSysTick();
	threadQueue[i].dueTick = threadQueue[i].duePercentage * threadQueue[i].numberOfTicksNeeded + threadQueue[i].startTick;
}

void schedule()
{
	updateThreadsInfos();
	sortThreadQueue();
	updatePriorities();
}

void updateThreadsInfos()
{
	uint8_t i;
	for (i = 0; i < NUM_THREADS; i++)
	{
		threadQueue[i].dueEstimate = osKernelSysTick() + (threadQueue[i].numberOfTicksNeeded - threadQueue[i].totalTicks);
		threadQueue[i].laxityTime = threadQueue[i].dueTick - threadQueue[i].dueEstimate;
	}
}

void sortThreadQueue()
{
	quick_sort(threadQueue, 0, NUM_THREADS - 1);
}


// Função de Ordenação por Seleção
// Quick sort function
void quick_sort(ThreadInfo *a, int left, int right) {
    int i, j;
	ThreadInfo x, y;
     
    i = left;
    j = right;
    x = a[(left + right) / 2];
     
    while(i <= j) {
        while(a[i].laxityTime < x.laxityTime && i < right) {
            i++;
        }
        while(a[j].laxityTime > x.laxityTime && j > left) {
            j--;
        }
        if(i <= j) {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }
     
    if(j > left) {
        quick_sort(a, left, j);
    }
    if(i < right) {
        quick_sort(a, i, right);
    }
}

void updatePriorities()
{
	uint8_t i;
	for (i = 1; i < NUM_THREADS; i++)
		threadQueue[i].dynamicPrio--;
}


void mailMan()
{

}