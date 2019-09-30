/*============================================================================
 *                    Exemplos de utilização do Kit
 *              EK-TM4C1294XL + Educational BooterPack MKII 
 *---------------------------------------------------------------------------*
 *                    Prof. André Schneider de Oliveira
 *            Universidade Tecnológica Federal do Paraná (UTFPR)
 *===========================================================================
 * Autores das bibliotecas:
 * 		Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 * 		Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 * 		Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 * 		Mariana Carrião        - <mcarriao@alunos.utfpr.edu.br>
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


//To print on the screen
tContext sContext;

// Floors
//uint16_t floors_x[] = {0, 0, 0, 0, 0}, floors_y[] = {127-FLOOR_HEIGHT,};

// Ladders
//uint16_t ladders_x[] = {}, ladders_y[] = {};

// Enemies
uint16_t enemies_x[] = {20, 65}, enemies_y[] = {80, 45};

// Itens
uint16_t itens_x[] = {20,80,50}, itens_y[] = {80,30,100};

// Eddie
uint16_t eddie_x = 64, eddie_y = 64;



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


/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	init_all();
	init_display();
	
	while(1){
		Eddie(sContext);
	
		PainelDeInstrumentos(sContext);
		
		Enemy(sContext);
		Item(sContext);
	}	
}
