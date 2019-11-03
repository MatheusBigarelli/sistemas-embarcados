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
#include <stdint.h>
#include "grlib/grlib.h"

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

#include "rgb.h"
#include "cfaf128x128x16.h"
#include "opt.h"
#include "buttons.h"
#include "buzzer.h"
#include "joy.h"
#include "led.h"

#include "thread.h"

#include "uart.h"
#include "mypwm.h"
#include "utils.h"


//To print on the screen
tContext sContext;

/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	#if SIMULADOR == 0
	// cfaf128x128x16Init();
	// joy_init();
	// button_init();
	// rgb_init();
	// opt_init();
	// led_init();
	//PLL_Init();
	//SysTick_Init();
	//GPIO_Init();
	//UART_init();
	pwmInit();
	#endif
}
/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	// osKernelInitialize();
	
	// createThreads();
	// createTimer();

	//Initializing all peripherals
	// init_all();

	// osKernelStart();


	//------------------------------------
	uint32_t configuration;

	init_all();
	configuration = PWM_GEN_MODE_DOWN |
					PWM_GEN_MODE_NO_SYNC;// |
//					PWM_GEN_MODE_DBG_RUN |
//					PWM_GEN_MODE_GEN_NO_SYNC;

	pwmClockSet(PWM_SYSCLK_DIV_64);
	pwmGenConfigure(configuration);
	pwmPulseWidthSet(1, PWM2_LOAD/2); // 50%
	pwmGenPeriodSet(65535);
	pwmGenEnable();
	pwmOutputEnable();
}
