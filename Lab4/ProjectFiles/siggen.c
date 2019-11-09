#include "siggen.h"

void SignalGenerator(const void* args)
{
	/**
	 * O sinal gerado deverá ser uma função do tipo
	 * y = x(n)
	 * 
	 * O sinal deverá ser periódico, de período N,
	 * que será 2^B, onde B é o número de bits do
	 * tipo da variável n, por exemplo, se n for uma
	 * do tipo uint8_t, B = 8 e N = 256.
	 * 
	 * O período total da onda será então o tempo entre
	 * mudanças no dutycycle, timeForDCCHange, vezes
	 * N valores dentro de um período da onda, o que
	 * nos dará um valor em microsegundos do período da
	 * onda. Assim, podemos conseguir a frequência da
	 * função gerada f = 10^6 / (timeForDCChange * N).
	 * A constante 10^6 deve-se ao fato do valor
	 * timeForDCChange estar em us.
	 * */
	uint8_t waveform = SINUSOIDAL;
	uint16_t n = 0, k = 1, adjust = 9; // Os valores de k e adjust serão explicados abaixo.
	uint32_t tick; // Último tick pego (último tempo analisado).
	uint32_t timeForDCChange = 10; // Tempo entre duas mudanças no dutycycle em us.
	float dutyCycle = 0.05;
	setPeriod(1000);
	tick = osKernelSysTick();

	/**
	 * Vamos utilizar um período base de espera de 10 us
	 * e um número de valores N de 2^16 na onda.
	 * 
	 * Até aqui temos um período base de onda total
	 * T = 10 us * 65536 ~= 655 ms
	 * O que resulta numa frequência de aproximadamente
	 * 1,52 Hz.
	 * 
	 * Para aumentar a frequência, então, basta usar um
	 * multiplicador junto à frequência base fb.
	 * 
	 * f = fb * k = 1,52 * k
	 * 
	 * Experimentalmente, verificou-se que a frequência
	 * base está dividida aproximadamente por 9.
	 * */
	while (1)
	{
		// Checagem de sinal para troca de parâmetros
		osSignalWait(SIG_WAVEFORM_CHANGE, SIG_INSTANT_CHECK_TIME);
		// Loop de espera.
		// Como o timer não tem resolução menor que 1 ms,
		// teremos de usar espera ocupada com base no 
		// tick do kernel para fazer a temporização do
		// gerador de sinal.
		while ((osKernelSysTick() - tick) < osKernelSysTickMicroSec(timeForDCChange)) ;
		tick = osKernelSysTick();
		// osSignalWait(SIG_UPDATE_PWM, osWaitForever);
		#if SIMULADOR == 0
		setDutyCycle(dutyCycle);
		#endif

		dutyCycle = updateDutyCycle(waveform, n, k*adjust);

		n++;
	}
}

float updateDutyCycle(uint8_t waveform, uint16_t n, uint16_t k)
{
	float dutyCycle = 0.5, increment = 0.01;
	// O valor de n deve ser atualizado para corresponder
	// à frequência desejada.
	// Como a onda deve ser periódica, a multiplicação deve
	// ser em módulo N (o período base da onda).
	// O cast elimina os bits mais significativos da
	// multiplicação nos dando o módulo N (16 bits).
	n = (uint16_t) (k * n);
	switch (waveform)
	{
		case SINUSOIDAL:
			dutyCycle = (1+sin(2*3.1415 * n/N)) / 2.0;
			break;

		case TRIANGULAR:
			if (dutyCycle <= 0.1)
				dutyCycle += increment;
			if (0.90 <= dutyCycle)
				dutyCycle = -increment;
			break;

		case TRAPEZOIDAL:
			/****************************
			 *  Número de fases: 4
			 * 		- Up
			 * 		- Corte
			 * 		- Down
			 * 		- Corte
			 * 
			 *  256/4 = 2**8 / 2**2 = 2**6 = 64
			 ****************************/
			if (0 < n && n < N/4.0) // Um quarto da onda a amplitude aumenta.
				dutyCycle += increment;
			// else if (N/4 <= n && n < N*2/4) // O segundo quarto se mantém.
			// 	NOP;
			else if (N*2.0/4.0 <= n && n < N*3.0/4.0) // No terceiro quarto a amplitude diminui.
				dutyCycle -= increment;
			// else if (N*3/4 <= n && n < N) // No último quarto a amplitude se mantém novamente.
			// 	NOP;
			if (n == 0)
				dutyCycle = 0.05; // Linha necessária para a precisão do float não
								  // distorcer a onda.
			break;

		case SQUARE:
			// Metade do período alta e metade baixa.
			if (n > N/2)
				dutyCycle = 0.95;
			else
				dutyCycle = 0.05;
			break;

		case SAWTOOTH:
			// Cresce linearmente e volta para 0 após o overflow
			// da variável de 16 bits.
			dutyCycle = n / N;
	}

	return dutyCycle;
}

void timerCallback(const void* args)
{
	// osSignalSet(tidSignalGenerator, SIG_UPDATE_PWM);
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

void makeSinusoid(void)
{
	uint16_t n = 0;
	float f = 50;
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


