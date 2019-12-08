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
	
	//Escreve menu lateral:
	// GrStringDraw(&sContext,"Exemplo EK-TM4C1294XL", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	// GrStringDraw(&sContext,"---------------------", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	// GrStringDraw(&sContext,"RGB", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	// GrStringDraw(&sContext,"ACC", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	// GrStringDraw(&sContext,"TMP", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	// GrStringDraw(&sContext,"OPT", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	// GrStringDraw(&sContext,"MIC", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	// GrStringDraw(&sContext,"JOY", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	// GrStringDraw(&sContext,"BUT", -1, 0, (sContext.psFont->ui8Height+2)*8, true);

}





/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void)
{
	Gantt_Info* info;
	
	osEvent event;

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
	initThreadsInfo();
	while (true)
	{
		schedule();
		
		// Manda informações para onde precisa.
		// HINT: send info display uart.
		//mailMan();
//		event = osSignalWait(SIG_GANTT, 0);
//		if(event.status == osEventSignal){
//			// Recebeu SIG_GANTT
//			// Envia as informações necessárias para thread UART montar Gantt
//			info = osMailAlloc(qidUartMailQueue, osWaitForever);
//			info->ids[0] = 0;
//			info->durations[0] = 100;
//			osMailPut(qidUartMailQueue, info);
//		}
	}

	return 0;
}

//Baseado nas prioridades internas aplica as prioridades do CMSIS = {osPriorityNormal, osPriorityIdle}
osThreadId aplyPriorityFromCMSIS()
{
    int i, lowestPrio = 999;
    osThreadId lowestId;
    // Lembrar que a prioridade interna de menor valor eh a mais alta
    for(i = 0; i < TOTAL_THREADS - 1; i++)
    {
        if(threadsInfo[i].currentState != WAITING && threadsInfo[i].dinamicPriority < lowestPrio)//Se thread estiver esperando nao considera ela
        {
            lowestId = threadsInfo[i].id;
            lowestPrio = threadsInfo[i].dinamicPriority;
        }
    }
    
    return lowestId; // Retorna osThreadId da tarefa mais prioritaria
}
uint32_t deltaInTicks = 0;
void updatePriority()
{
    int i;
    uint32_t expectedTickOfDeadline;
    for(i=0;i<TOTAL_THREADS - 1;i++)
    {
		if(threadsInfo[i].currentState == READY)//Se thread estiver pronta para entrar, mas ainda nao rodando
        {
            expectedTickOfDeadline = osKernelSysTick() + threadsInfo[i].durationInTicks*(1 - threadsInfo[i].executionPercent);
            if(expectedTickOfDeadline > threadsInfo[i].tickOfDeadline)
            {
                threadsInfo[i].laxityTimeInTicks = 0;
            }
            else
            {
                threadsInfo[i].laxityTimeInTicks = threadsInfo[i].tickOfDeadline - expectedTickOfDeadline;
            }
            threadsInfo[i].dinamicPriority -= threadsInfo[i].laxityTimeInTicks; 
        }
	}
}
void schedule()
{
    osThreadId lowestId;
    uint32_t initialTick;
    updatePriority();
    lowestId = aplyPriorityFromCMSIS();
    // Inicia execucao da tarefa lowestId
    initialTick = osKernelSysTick();
	threadSwitch(lowestId);
    deltaInTicks = osKernelSysTick() - initialTick;
    // tarefa terminou de executar ou entregou o processador
	
}

void mailMan()
{

}