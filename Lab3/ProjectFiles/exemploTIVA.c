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

#include "eddie.h"
#include "painel.h"



extern void Eddie(void const *args);
extern void PainelDeInstrumentos(void const *args);
extern void Enemy(void const *args);
extern void Item(void const *args);

//To print on the screen
tContext sContext;

// Floors
//uint16_t floors_x[] = {0, 0, 0, 0, 0}, floors_y[] = {127-FLOOR_HEIGHT,};

// Ladders
//uint16_t ladders_x[] = {}, ladders_y[] = {};

// Enemies
int16_t enemies_x[] = {20, 65}, enemies_y[] = {80, 45};

// Itens
int16_t itens_x[] = {20,80,50}, itens_y[] = {80,30,100};

// Eddie
int16_t eddie_x = 64, eddie_y = 64;


// Seguran�a de recursos compartilhados
osMutexId context_mutex;
osMutexDef(context_mutex);



osThreadDef(Eddie, osPriorityNormal, 1, 0);
osThreadDef(PainelDeInstrumentos, osPriorityNormal, 1, 0);
osThreadDef(Enemy, osPriorityNormal, 1, 0);
osThreadDef(Item, osPriorityNormal, 1, 0);


/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(void){
	cfaf128x128x16Init();
	joy_init();
	buzzer_init(); 
	button_init();
	rgb_init();
}

void init_display(void){
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
}


void createThreads(void)
{
	osThreadCreate(osThread(Eddie), NULL);
	osThreadCreate(osThread(PainelDeInstrumentos), NULL);
	osThreadCreate(osThread(Enemy), NULL);
	osThreadCreate(osThread(Item), NULL);
}

void createMutex(void)
{
	context_mutex = osMutexCreate(osMutex(context_mutex));
}

/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	osKernelInitialize();
	
	init_all();
	init_display();	
	
	createThreads();
	createMutex();
	
	osKernelStart();
	osWait(osWaitForever);
}
