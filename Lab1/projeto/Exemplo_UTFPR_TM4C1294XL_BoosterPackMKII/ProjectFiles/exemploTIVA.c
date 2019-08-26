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

#include <math.h>

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

#include "cfaf128x128x16.h"
#include "buttons.h"
#include "airplane.h"
#include "buffalo.h"
#include "bus.h"
#include "car.h"


const uint8_t DISPLAY_WIDTH = 128;
const uint8_t DISPLAY_HEIGHT = 128;
const uint8_t IMAGE_WIDTH = 48;
const uint8_t IMAGE_HEIGHT = 32;


extern void SysTick_Init(void);
extern void SysTick_Wait1ms(uint32_t time);

//To print on the screen
tContext sContext;
uint32_t background = ClrBlack;

unsigned char* images[] = {airplane_image, car_image};
int current_image = 0;
typedef struct
{
		int height;
		int width;
		unsigned char data[16384];
} Image;

// States:
//  0     1     2    3    4   5  6  7  8   9  10
// 1/32  1/16  1/8  1/4  1/2  1  2  4  8  16  32
int going_up = 1;

Image image;


void invertColors(void);
void drawImage(void);
void updateImageState(void);
void clearDisplay(void);

extern int Test(void);
extern void resizeImageASM(int h1, int w1, int h2, int w2);
void resizeImage(void);
//void resizeImage3(int h1, int w1, int h2, int w2);

/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init(){
	cfaf128x128x16Init();
	button_init();
	SysTick_Init();
	
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
	
	// Initializing buttons, display and SysTick
	init();
	// Sidelong menu creation
	image.height = 32;
	image.width = 48;

  while(1){
			resizeImage();
			clearDisplay();
			drawImage();
			
			if (button_read_s1())
					current_image = 1 - current_image;
			if (button_read_s2())
					invertColors();
		
			SysTick_Wait1ms(500);
	}
}


void resizeImage(void)
{
		static float resize_factor = 1;
		int h1 = 32, h2 = h1*resize_factor, w1 = 48, w2 = 48*resize_factor;
	
	
		image.height = h2;
		image.width = w2;
	
		resizeImageASM(h1, w1, h2, w2);
		//resizeImage3(h1, w1, h2, w2);


	
		if (going_up)
				resize_factor+=0.25;
		else
				resize_factor-=0.25;
		if (resize_factor >= 3)
				going_up = 0;
		if (resize_factor <= 0.50)
				going_up = 1;
}

//void resizeImage3(int h1, int w1, int h2, int w2)
//{
//		int i, j;
//		int x_ratio = (int)((w1<<16)/w2) +1;
//    int y_ratio = (int)((h1<<16)/h2) +1;
//    int x2, y2 ;
//	
//    for (i=0;i<h2;i++)
//		{
//        for (j=0;j<w2;j++)
//				{
//            x2 = ((j*x_ratio)>>16) ;
//            y2 = ((i*y_ratio)>>16) ;
//            image.data[(i*w2)+j] = images[current_image][(y2*w1)+x2] ;
//        }                
//    }
//}


void drawImage(void)
{
		uint8_t x, y;
		for (y = 0; y < image.height; y++)
		{
				for (x = 0; x < image.width	; x++)
				{
						if (image.data[image.width*y + x] < 0xAA)
						{
								invertColors();
								GrPixelDraw(&sContext, x, y);
								invertColors();
						}
						else
								GrPixelDraw(&sContext, x, y);
				}
		}
}


void invertColors(void)
{	
		GrContextForegroundSet(&sContext, background);
		background = ~background & 0x00FFFFFF;
		GrContextBackgroundSet(&sContext, background);
}


void clearDisplay(void)
{
		uint8_t x, y;
		// Need to invert colors, because GrPixelDraw()
		// draws pixel with the foreground color
		invertColors();
		for (y = 0; y < DISPLAY_HEIGHT; y++)
		{
				for (x = 0; x < DISPLAY_WIDTH; x++)
				{
						GrPixelDraw(&sContext, x, y);
				}
		}
		// Setting colors back to normal
		invertColors();
}