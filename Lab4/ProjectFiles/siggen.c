#include "siggen.h"

void SignalGenerator(void const* args)
{	
	double dutyCycle = 0.50, increment = 0.01; // Define duty cycle
	
    PWM0_CC_R = 0x100; // Divide por 2 o clock do sistema (Ex.: 80/2=40Mhz)
    PWM0_2_CTL_R = 0x0;
    PWM0_2_GENB_R = 0x80c;
    PWM0_2_LOAD_R = 1600; // Usando o modo count down
    PWM0_2_CMPB_R = PWM0_2_LOAD_R * (1 - dutyCycle);

	
	makeSinosoid();
}

void makeSawTooth(void)
{
	uint16_t n = 0;
	double dutyCycle; // Define duty cycle
	float f = 1;
	float t = 1.0/f;
	uint16_t numSamples = 1000;
	
    while (true)
    {	
		osDelay(t/numSamples);
		dutyCycle = (double)(n)/(double)numSamples;
		n = (n+1) % numSamples;
			
		PWM0_2_CMPB_R = PWM0_2_LOAD_R * (1 - dutyCycle);
    }
}

void makeSinosoid(void)
{
	uint16_t n = 0;
	float f = WAVE_FREQUENCY(50);
	double dutyCycle = 0.50; // Define duty cycle
    while (true)
    {		
		dutyCycle = (sin(2*3.1415*f/65536*n) + 1)/2;
		
		n++;
			
		PWM0_2_CMPB_R = PWM0_2_LOAD_R * (1 - dutyCycle);
    }
}



void makeSquare(void)
{
	
	
}


