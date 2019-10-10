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


#include "ustdlib.h"
#include "tiva-random.h"

#include "rgb.h"
#include "cfaf128x128x16.h"
#include "buttons.h"
#include "joy.h"
#include "buzzer.h"

#include "eddie.h"
#include "painel.h"
#include "enemy.h"
#include "item.h"



extern void Eddie(void const *args);
extern void PainelDeInstrumentos(void const *args);
extern void Enemy(void const *args);
extern void ItemThread(void const *args);

//To print on the screen
tContext sContext;

// Floors
// Floors never change places, so they are calculated on the go.

// Ladders
//int16_t ladders_x[NUM_LADDERS] = {90, 30, 100, 10}, ladders_y[NUM_LADDERS] = {LADDER_Y(FLOOR(3)), LADDER_Y(FLOOR(2)), LADDER_Y(FLOOR(1)), LADDER_Y(FLOOR(4))};
	
// Enemies
//int16_t enemies_x[] = {20, 65}, enemies_y[] = {ENEMY_Y(FLOOR(2)), ENEMY_Y(FLOOR(3))};

// Itens
Item itens[NUM_ITENS];
//int16_t itens_x[] = {20,80,50}, itens_y[] = {ITEM_Y(FLOOR(4)), ITEM_Y(FLOOR(3)), ITEM_Y(FLOOR(2))};

// Eddie
int16_t eddie_x = EDDIE_BASE_X, eddie_y = EDDIE_BASE_Y;


// Seguran�a de recursos compartilhados
osMutexId context_mutex;
osMutexDef(context_mutex);



osThreadDef(Eddie, osPriorityNormal, 1, 0);
osThreadDef(PainelDeInstrumentos, osPriorityNormal, 1, 0);
osThreadDef(Enemy, osPriorityNormal, 1, 0);
osThreadDef(ItemThread, osPriorityNormal, 1, 0);


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
	osKernelInitialize();
	
	usrand(RandomSeed());
	
	init_all();
	init_display();	
	
	createThreads();
	createMutex();
	
	osKernelStart();
	osWait(osWaitForever);
}
