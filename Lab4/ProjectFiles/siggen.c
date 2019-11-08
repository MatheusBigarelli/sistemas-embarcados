#include "siggen.h"

void SignalGenerator(const void* args)
{
	uint8_t n = 0;
	float dutyCycle = 0.0, increment = 0.01;
	setPeriod(1000);
	while (1)
	{
		osSignalWait(SIG_UPDATE_PWM, osWaitForever);
		#if SIMULADOR == 0
		setDutyCycle(dutyCycle);
		#endif

		/****************************
		 *  Onda triangular.
		 ****************************/
		// dutyCycle += increment;
		// if (dutyCycle <= 0.1 || 0.90 <= dutyCycle)
		// 	increment = -increment;


		/****************************
		 *  Onda senoidal.
		 ****************************/
		// dutyCycle = (1+sin(2*3.1415*5 * n/256.0)) / 2.0;
		// n++;

		
		/****************************
		 *  Onda quadrada.
		 ****************************/
		// if (n > 128)
		// 	dutyCycle = 0.95;
		// else
		// 	dutyCycle = 0.05;
		// n++;

		
		/****************************
		 *  Onda trapezoidal.
		 *  Número de fases: 4
		 * 		- Up
		 * 		- Corte
		 * 		- Down
		 * 		- Corte
		 * 
		 *  256/4 = 2**8 / 2**2 = 2**6 = 64
		 *  Valor médio: 0.50
		 * 	Valor de corte alto: 0.80?
		 *  Valor de corte baixo: 0.30?
		 ****************************/
		// if (0 < n && n < 64)
		// 	dutyCycle += increment;
		// // else if (64 <= n && n < 128)
		// // 	NOP;
		// else if (128 <= n && n < 192)
		// 	dutyCycle -= increment;
		// // else if (192 <= n && n < 256)
		// // 	NOP;
		// if (n == 0)
		// 	dutyCycle = 0.05; // Linha necessária para a precisão do float não
		// 					  // distorcer a onda.
		// n++;

		/****************************
		 *  Onda dente de serra.
		 ****************************/
		dutyCycle = n/256.0;
		n++;
	}
}

void timerCallback(const void* args)
{
	osSignalSet(tidSignalGenerator, SIG_UPDATE_PWM);
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
		n = (n + 1) % numSamples;
		setDutyCycle(dutyCycle);
	}
}

void makeSinosoid(void)
{
	uint16_t n = 0;
	float f = WAVE_FREQUENCY(50);
	double dutyCycle = 0.50; // Define duty cycle

	setPeriod(1000);
    while (true)
    {		
		dutyCycle = (sin(2*3.1415*f/65536*n) + 1)/2;	
		n++;
		setDutyCycle(dutyCycle);
    }
}



void makeSquare(void)
{
	float dutyCycle = 1;
	setPeriod(1000);
	while(true)
	{
		setDutyCycle(0.1);
		// osDelay(10);
		setDutyCycle(0.9);
		// osDelay(10);
	}
}


void makeTriangular(void)
{
	float dutyCycle = 0, increment = 0.01;
	setPeriod(1000);
	while(true)
	{
		setDutyCycle(dutyCycle);
		dutyCycle += increment;
		if (dutyCycle <= 0.01 || 0.99 <= dutyCycle)
			increment = -increment;
		osDelay(10);
	}
}


