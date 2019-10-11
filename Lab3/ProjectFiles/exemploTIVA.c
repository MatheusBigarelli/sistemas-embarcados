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

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

#include "rgb.h"
#include "cfaf128x128x16.h"
#include "buttons.h"
#include "joy.h"
#include "buzzer.h"

#include "draw.h"
#include "util.h"



//To print on the screen
tContext sContext;

//Threads
osThreadDef(Entrada, osPriorityNormal, 1, 0);
osThreadDef(Eddie, osPriorityNormal, 1, 0);
osThreadDef(Inimigos, osPriorityNormal, 1, 0);
osThreadDef(ItensBrilhantes, osPriorityNormal, 1, 0);
osThreadDef(PainelDeInstrumentos, osPriorityNormal, 1, 0);
osThreadDef(Saida, osPriorityNormal, 1, 0);

//Mutex
osMutexId mid_displayMutex;
osMutexDef(displayMutex);

/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	cfaf128x128x16Init();
	joy_init();
	buzzer_init(); 
	button_init();
	rgb_init();
}

void init_display(){
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
}

void init_threads()
{
	osThreadCreate(osThread(Entrada), NULL);
	osThreadCreate(osThread(Eddie), NULL);
	osThreadCreate(osThread(Inimigos), NULL);
	osThreadCreate(osThread(ItensBrilhantes), NULL);
	osThreadCreate(osThread(PainelDeInstrumentos), NULL);
	osThreadCreate(osThread(Saida), NULL);
	
	mid_displayMutex = osMutexCreate(osMutex(displayMutex));
}

/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	int counter=0;
	char* pbuffer;
	init_all();
	init_display();
	initMap();	
	osKernelInitialize();
	init_threads();
	osKernelStart();
}
