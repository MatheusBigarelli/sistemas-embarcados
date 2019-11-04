#include "siggen.h"

void SignalGenerator(void const* args)
{
    // uint32_t configuration;

    // configuration = PWM_GEN_MODE_DOWN;

    // pwmClockSet(PWM_SYSCLK_DIV_1);
    // pwmGenConfigure(configuration);
    // pwmGenPeriodSet(120000000 / 10000); // 10kHz PWM
    // pwmPulseWidthSet(0, PWM2_LOAD / 100); // 50%
    // pwmGenEnable();
    // pwmOutputEnable();
	
	float dutyCycle = 0.10, increment = 0.01; // Define duty cycle

    PWM0_CC_R = 0x100; // Divide por 2 o clock do sistema(80/2=40Mhz)
    PWM0_2_CTL_R = 0x0;
    PWM0_2_GENB_R = 0x80c;
    PWM0_2_LOAD_R = 4000; // Usando o modo count down
    PWM0_2_CMPB_R = PWM0_2_LOAD_R * (1 - dutyCycle);

    PWM0_2_CTL_R = 0x1;
    PWM0_ENABLE_R = 0x1 << 5; // Ativa pwm5

    while (true)
    {
		osDelay(100);
		dutyCycle += increment;
		if (dutyCycle <= 0.1 || dutyCycle >= 0.9)
			increment = -increment;
			
		PWM0_2_CMPB_R = PWM0_2_LOAD_R * (1 - dutyCycle);
    }
}



