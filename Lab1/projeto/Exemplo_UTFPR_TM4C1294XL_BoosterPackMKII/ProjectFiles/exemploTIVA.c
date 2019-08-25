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
uint8_t current_image = 0;
typedef struct
{
		uint8_t height;
		uint8_t width;
		unsigned char data[DISPLAY_WIDTH*DISPLAY_HEIGHT];
} Image;

// States:
//  0     1     2    3    4   5  6  7  8   9  10
// 1/32  1/16  1/8  1/4  1/2  1  2  4  8  16  32
uint8_t current_state = 5;
uint8_t going_up = 1;

Image image;


void invertColors(void);
void drawImage(void);
void updateImageState(void);
void clearDisplay(void);


extern void resizeImage(void);
void resizeImage2(void);

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
	
	// Initializing all peripherals
	init_all();
	// Sidelong menu creation
	init_sidelong_menu();
	image.height = 32;
	image.width = 48;

  while(1){
			resizeImage2();
			clearDisplay();
			drawImage();
			
//			if (button_read_s1())
//					invertColors();
//			
//			if (button_read_s2())
//					// Change image
//					// Alternates between 0 and 1;
//					current_image = 1 - current_image;
//			
			SysTick_Wait1ms(2000);
			

		//updateImageState();
	}
}


void resizeImage2(void)
{
		static float resize_factor = 1;
		int i, j, h1 = 32, h2 = h1*resize_factor, w1 = 48, w2 = 48*resize_factor;
    double px, py;
    double x_ratio = w1 / (double)w2;
    double y_ratio = h1 / (double)h2;

    for (i = 0; i < h2; i++)
    {
        for (j = 0; j < w2; j++)
        {
            px =  floor((j * x_ratio));
            py =  floor((i * y_ratio));
            image.data[(i * w2) + j] = images[current_image][(int) (py * w1 + px)];
        }
    }


			image.height = h2;
			image.width = w2;
	
		if (going_up)
				resize_factor+=0.25;
		else
				resize_factor-=0.25;
		if (resize_factor >= 3)
				going_up = 0;
		if (resize_factor <= 1)
				going_up = 1;
}


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


void updateImageState(void)
{
		if (going_up)
		{
			
				// Reached enlarging limit
				if (current_state == 10)
				{
						current_state = 9;
						going_up = 0;
					
						image.width *= 2;
						image.height *= 2;
						
						if (image.height > 128)
							image.height = 128;
						if (image.width > 128)
							image.width = 128;
				}
		
				else
				{
						current_state++;
						image.width *= 2;
						image.height *= 2;
						
						if (image.height > 128)
							image.height = 128;
						if (image.width > 128)
							image.width = 128;
					
						return;
				}
		}
		
		// Image is shrinking
		else
		{
				// Reached shrinking limit
				if (current_state == 0)
				{
						current_image = 1;
						going_up = 1;
					
						
						image.height /= 2;
						image.width /= 2;
						
						// Needs to be at least 1, because we will
						// multiply only, then it will get bigger again
						if (image.height == 0)
							image.height = 1;
						if (image.width == 0)
							image.width = 1;
					
						return;
				}
		
				else
				{
						image.height /= 2;
						image.width /= 2;
						
						// Needs to be at least 1, because we will
						// multiply only, then it will get bigger again
						if (image.height == 0)
							image.height = 1;
						if (image.width == 0)
							image.width = 1;
						
						// Image being shrinked
						current_state--;
				}
		}
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