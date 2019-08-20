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
	bool s1_pressed, s2_pressed;
	
	//Initializing all peripherals
	init_all();
	//Sidelong menu creation
	init_sidelong_menu();

  while(1){
			for (y = 0; y < 64; y++)
			{
					for (x = 0; x < 96; x++)
					{
							if ((eprom[3*96*y + 3*x + 0] + eprom[3*96*y + 3*x + 1] + eprom[3*96*y + 3*x + 2])/3 < 0xAA)
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
			{
					counter++;
					if (counter > 2)
					{
							counter = 0;
							invertColors();
					}		
			}
			//if (button_read_s2())
					//changeImage();
	}
}


void invertColors(void)
{	
		GrContextForegroundSet(&sContext, background);
		background = ~background & 0x00FFFFFF;
		GrContextBackgroundSet(&sContext, background);
}