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
 *--------------------------------
 --------------------------------------------*/

#include "cfaf128x128x16.h"
#include "airplane.h"
#include "buttons.h"

#define LED_A      0
#define LED_B      1
#define LED_C      2
#define LED_D      3
#define LED_CLK    7

//To print on the screen
tContext sContext;


bool validPixel(uint8_t x, uint8_t y);
void invertColors(void);
uint32_t background = ClrBlack;

/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/
void init_display(){
	cfaf128x128x16Init();
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
	bool s1_pressed;
	uint16_t counter = 0;
	button_init();
	init_display();

  while(1){
			for (y = 0; y < 64; y++)
			{
					for (x = 0; x < 96; x++)
					{
							//if (validPixel(x, y))
							if ((eprom[3*96*y + 3*x + 0] + eprom[3*96*y + 3*x + 1] + eprom[3*96*y + 3*x + 2])/3 < 0xAA)
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


bool validPixel(uint8_t x, uint8_t y)
{
		if (((eprom[3*96*y + 3*x + 0] + eprom[3*96*y + 3*x + 1] + eprom[3*96*y + 3*x + 2])/3 < 0x41) && (sContext.ui32Foreground == ClrBlack))
				return true;
		
		if (((eprom[3*96*y + 3*x + 0] + eprom[3*96*y + 3*x + 1] + eprom[3*96*y + 3*x + 2])/3 > 0xDD) && (sContext.ui32Foreground == ClrWhite))
				return true;
		
		if (x % 6 == 0)
			return true;
		
		return false;
}


void invertColors(void)
{	
		GrContextBackgroundSet(&sContext, (~background & 0x00FFFFFF));
		GrContextForegroundSet(&sContext, (background & 0x00FFFFFF));
}