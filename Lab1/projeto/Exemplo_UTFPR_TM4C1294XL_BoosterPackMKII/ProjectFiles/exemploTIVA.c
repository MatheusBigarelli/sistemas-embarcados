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

#include "cfaf128x128x16.h"
#include "buttons.h"
#include "airplane.h"
#include "buffalo.h"
#include "bus.h"
#include "car.h"

extern void SysTick_Init(void);
extern void SysTick_Wait1ms(uint32_t time);

//To print on the screen
tContext sContext;
uint32_t background = ClrBlack;

void invertColors(void);

/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	cfaf128x128x16Init();
	button_init();
	SysTick_Init();
}

void init_sidelong_menu(){
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

	uint8_t x, y;
	uint16_t counter;
	
	uint8_t iterator = 0;

	unsigned char* images[] = {airplane_image, buffalo_image, bus_image, car_image};
		
	bool s1_pressed, s2_pressed;
	
	//Initializing all peripherals
	init_all();
	//Sidelong menu creation
	init_sidelong_menu();

  while(1){
			for (y = 0; y < 32; y++)
			{
					for (x = 0; x < 48; x++)
					{
							if (images[iterator][48*y + x] < 0xAA)
							{
									invertColors();
									GrPixelDraw(&sContext, x+16, y+32);
									invertColors();
							}
							else
									GrPixelDraw(&sContext, x+16, y+32);
					}
			}
			
			s1_pressed = button_read_s1();
			
			if (s1_pressed)
					invertColors();
			
			if (button_read_s2())
			{
					// Change image
					iterator = (iterator + 1) % 4;
					SysTick_Wait1ms(500);
			}
	}
}


void invertColors(void)
{	
		GrContextForegroundSet(&sContext, background);
		background = ~background & 0x00FFFFFF;
		GrContextBackgroundSet(&sContext, background);
}