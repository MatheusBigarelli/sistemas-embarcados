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
#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"


#include "rgb.h"
#include "cfaf128x128x16.h"
#include "buttons.h"
#include "joy.h"
#include "buzzer.h"

#include "draw.h"
#include "threads.h"


extern void EddieThread(void const *args);
extern void PainelDeInstrumentos(void const *args);
extern void EnemyThread(void const *args);
extern void ItemThread(void const *args);

//To print on the screen
tContext sContext;

//Threads
osThreadId Entrada_tid;
osThreadDef(Entrada, osPriorityNormal, 1, 0);
osThreadId Eddie_tid;
osThreadDef(Eddie, osPriorityNormal, 1, 0);
osThreadId Inimigos_tid;
osThreadDef(Inimigos, osPriorityNormal, 1, 0);
osThreadId ItensBrilhantes_tid;
osThreadDef(ItensBrilhantes, osPriorityNormal, 1, 0);
osThreadId PainelDeInstrumentos_tid;
osThreadDef(PainelDeInstrumentos, osPriorityNormal, 1, 0);
osThreadId Saida_tid;
osThreadDef(Saida, osPriorityNormal, 1, 0);

//Mutex
osMutexId mid_displayMutex;
osMutexDef(displayMutex);

//Timer
osTimerId timer_item;
osTimerDef(clear_timer, replaceItem);

/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/
void init_display(){
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
}
void init_threads()
{
	Entrada_tid = osThreadCreate(osThread(Entrada), NULL);
	Eddie_tid = osThreadCreate(osThread(Eddie), NULL);
	Inimigos_tid = osThreadCreate(osThread(Inimigos), NULL);
	ItensBrilhantes_tid = osThreadCreate(osThread(ItensBrilhantes), NULL);
  PainelDeInstrumentos_tid = osThreadCreate(osThread(PainelDeInstrumentos), NULL);
	Saida_tid = osThreadCreate(osThread(Saida), NULL);
	
	mid_displayMutex = osMutexCreate(osMutex(displayMutex));
	
	timer_item = osTimerCreate(osTimer(clear_timer),osTimerOnce,NULL);
}
void init_all(){
	cfaf128x128x16Init();
	joy_init();
	buzzer_init(); 
	button_init();
	init_display();
	
	initMap();	
	
	osKernelInitialize();
	init_threads();
}


void createThreads(void)
{
	osThreadCreate(osThread(EddieThread), NULL);
	osThreadCreate(osThread(PainelDeInstrumentos), NULL);
	osThreadCreate(osThread(EnemyThread), NULL);
	osThreadCreate(osThread(ItemThread), NULL);
}

void createMutex(void)
{
	context_mutex = osMutexCreate(osMutex(context_mutex));
}

/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
<<<<<<< HEAD
	osKernelInitialize();
	
	init_all();
	init_display();	
	
	createThreads();
	createMutex();
	
	osKernelStart();
	osWait(osWaitForever);
=======
	init_all();		
	osKernelStart();
>>>>>>> biga
}
