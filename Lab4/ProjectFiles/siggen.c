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
	uint8_t waveform = TRIANGULAR;
	uint16_t n = 0, k = 1, adjust = 9; // Os valores de k e adjust serão explicados abaixo.
	uint32_t tick; // Último tick pego (último tempo analisado).
	uint32_t timeForDCChange = 10; // Tempo entre duas mudanças no dutycycle em us.
	float dutyCycle = 0.05;
	float frequency = 1.52;
	float amplitude = 3.3;
	osEvent event;
	SignalConfig_t* sigConfigMail;

	setPeriod(100);
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
		event = osMailGet(qidSigGenMailQueue, MAIL_INSTANT_CHECK_TIME);
		if (event.status == osEventMail)
		{
			sigConfigMail = event.value.p;
			if (sigConfigMail != NULL)
			{
				if (sigConfigMail->changedParameter == WAVEFORM)
				{
					waveform = updateWaveformPWM(waveform, sigConfigMail->waveform);
				}

				if (sigConfigMail->changedParameter == FREQUENCY)
				{
					k = updateFrequencyPWM(&frequency, sigConfigMail->frequency, k);
				}

				if (sigConfigMail->changedParameter == AMPLITUDE)
				{
					amplitude = updateAmplitudePWM(amplitude, sigConfigMail->amplitude);
				}

				osMailFree(qidSigGenMailQueue, sigConfigMail);
			}
		}

		// Loop de espera.
		// Como o timer não tem resolução menor que 1 ms,
		// teremos de usar espera ocupada com base no 
		// tick do kernel para fazer a temporização do
		// gerador de sinal.
		while ((osKernelSysTick() - tick) < osKernelSysTickMicroSec(timeForDCChange)) ;
		tick = osKernelSysTick();
		#if SIMULADOR == 0
		setDutyCycle(dutyCycle);
		#endif

		dutyCycle = updateDutyCycle(waveform, amplitude, n, k*adjust);

		n++;
	}
}

float updateDutyCycle(uint8_t waveform, float amplitude, uint16_t n, uint16_t k)
{
	float dutyCycle = 0.5;
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
			if (n < N/2.0)
				dutyCycle = 2 * n / N;
			else
				dutyCycle = 2 - 2 * n / N;
			break;

		case TRAPEZOIDAL:
			/****************************
			 *  Número de fases: 4
			 * 		- Up
			 * 		- Corte
			 * 		- Down
			 * 		- Corte
			 * 
			 *  65536/4 = 2**16 / 2**2 = 2**14
			 ****************************/
			if (0 < n && n < N/4.0) // Um quarto da onda a amplitude aumenta.
				dutyCycle = 4 * n / N;
			else if (N/4 <= n && n < N*2/4) // O segundo quarto se mantém.
				dutyCycle = 0.95;
			else if (N*2.0/4.0 <= n && n < N*3.0/4.0) // No terceiro quarto a amplitude diminui.
				dutyCycle = 3 - 4*n/N;
			else if (N*3/4 <= n && n < N) // No último quarto a amplitude se mantém novamente.
				dutyCycle = 0.05;
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

	dutyCycle = dutyCycle * amplitude / MAX_AMPLITUDE;
	return dutyCycle;
}

WAVEFORMS updateWaveformPWM(WAVEFORMS oldWaveform, WAVEFORMS newWaveform)
{
	if (SINUSOIDAL <= newWaveform && newWaveform <= SAWTOOTH)
	{
		return newWaveform;
	}

	return oldWaveform;
}

uint16_t updateFrequencyPWM(float* oldFrequency, float newFrequency, uint16_t oldK)
{
	uint16_t k;
	
	// Range de frequências inválidas
	if (newFrequency < 1.52 || newFrequency > 200)
		return oldK;

	*oldFrequency = newFrequency;
	k = newFrequency/1.52;
	return k;
}

float updateAmplitudePWM(float oldAmplitude, float newAmplitude)
{
	if (0 < newAmplitude && newAmplitude <= 33)
	{
		return newAmplitude;
	}

	return MAX_AMPLITUDE;
}

