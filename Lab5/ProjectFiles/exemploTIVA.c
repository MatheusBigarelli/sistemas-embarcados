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


//To print on the screen
tContext sContext;

void schedule();
void setupMail();

extern void floatToString(float value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros, uint8_t precision);
extern void intToString(int64_t value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros);
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
	int i;
	osEvent event;

	osKernelInitialize();

	#if SIMULADOR == 0
	init_all();
	init_sidelong_menu();
	#endif

	createThreads();
	createTimers();
	
	osKernelStart();

	tidMain = osThreadGetId();
	clearUART();
	initThreadsInfo();
	while (true)
	{
		schedule();
        setupMail();
        
		event = osSignalWait(SIG_GANTT, 0);
		if(event.status == osEventSignal){
			// Recebeu SIG_GANTT
            threadSwitch(tidUART);
		}
	}

	return 0;
}

//Baseado nas prioridades internas aplica as prioridades do CMSIS = {osPriorityNormal, osPriorityIdle}
osThreadId aplyPriorityFromCMSIS()
{
    int i, lowestPrio = 999;
    osThreadId lowestId;
    // Lembrar que a prioridade interna de menor valor eh a mais alta
    for(i = 0; i < TOTAL_MATH_THREADS; i++)
    {
        if(threadsInfo[i].currentState != WAITING && threadsInfo[i].staticPriority < lowestPrio)//Se thread estiver esperando nao considera ela
        {
            lowestId = threadsInfo[i].id;
            lowestPrio = threadsInfo[i].staticPriority;
        }
    }
    
    return lowestId; // Retorna osThreadId da tarefa mais prioritaria
}
//static void intToString(int64_t value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros);
// Funcao principal do escalonador
// Primeiro o escalonador "converte" as suas prioridades em prioridades que o RTOS consegue usar
// Após isso a tarefa prioritaria e o escalonador terao prioridade osPriorityNormal e todas as outras osPriorityIdle
// Apos o threadSwitch o escalonador tambem tera sua prioridade reduzida para osPriorityIdle, de modo que a tarefa prioritaria podera entao executar
void schedule()
{
    osThreadId lowestId;
    // Ira atualizar apenas na ocorrencia de secondary faults
    lowestId = aplyPriorityFromCMSIS();
    // Inicia execucao da tarefa lowestId
	threadSwitch(lowestId);
    // tarefa terminou de executar ou entregou o processador
	
}

void setupMail()
{
    int i;
    char buffer[3];
    volatile uint32_t startTick, endTick;
    if(remaingThreadToCompleteCycle == 0)
    {
        // Todas as tarefas ja executaram pelo menos uma vez, acumula suas informacoes para formar o Gantt
        for(i = 0; i< TOTAL_MATH_THREADS; i++)
        {
            startTick = ganttInfo[i].startTick;
            endTick = ganttInfo[i].endTick;
            intToString(startTick, buffer, 3, 10, 3);
            strcat(ganttInfo[i].ganttString, buffer);
            strcat(ganttInfo[i].ganttString, ",");
            intToString(endTick, buffer, 3, 10, 3);
            strcat(ganttInfo[i].ganttString, buffer);
            strcat(ganttInfo[i].ganttString, ":");
            ganttInfo[i].activations++;
        }
        
        remaingThreadToCompleteCycle = TOTAL_MATH_THREADS;
    }
}
