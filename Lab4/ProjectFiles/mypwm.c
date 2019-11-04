#include "mypwm.h"


/**************************************************
 * 
 * Funções de inicialização e configuração de HW.
 * 
***************************************************/
void pwmInit(void)
{
    configurePWMDependencies();
    configureGPIOPort();
}

void configureGPIOPort(void)
{
	SYSCTL_RCGCGPIO_R |= GPIO_RCGC_PORTG;
	while((SYSCTL_PRGPIO_R & (GPIO_RCGC_PORTG) ) != (GPIO_RCGC_PORTG) ){};
	
	GPIO_PORTG_AHB_AMSEL_R = 0x00;
	GPIO_PORTG_AHB_PCTL_R = 0x60; // PWM
	GPIO_PORTG_AHB_DIR_R = GPIO_PORTG_BITS;
	GPIO_PORTG_AHB_AFSEL_R = GPIO_PORTG_BITS;
	GPIO_PORTG_AHB_DEN_R = GPIO_PORTG_BITS;
}

void configurePWMDependencies(void)
{
    // Ativando clock do módulo do PWM.
    SYSCTL_RCGCPWM_R |= 0x1;
}



/**************************************************
 * 
 * Funções da API do PWM.
 * 
***************************************************/

void pwmGenConfigure(uint32_t configuration)
{
    // Zerando possíveis configurações feitas e aplicando
    // nova configuração.
    PWM2_CTL = (PWM2_CTL & ~ (PWM_X_CTL_MODE | PWM_X_CTL_DEBUG |
                              PWM_X_CTL_GENAUPD_M |
                              PWM_X_CTL_GENAUPD_M |
                              PWM_X_CTL_LOADUPD | PWM_X_CTL_CMPAUPD |
                              PWM_X_CTL_CMPAUPD) | // Até aqui tudo foi zerado
                            configuration); // Com o OU lógico com configuration,
                            // a configuração passada foi setada no registrador
                            // de controle do gerador

    
    if (configuration & PWM_X_CTL_MODE)
    {
        // Se estiver na configuração de UP/DOWN.
        PWM2_GENB = (PWM_X_GENB_ACTCMPBU_ONE |
                     PWM_X_GENB_ACTCMPBD_ZERO);
    }
    else
    {
        // Se estiver na configuração DOWN.
        PWM2_GENB = (PWM_X_GENB_ACTLOAD_ONE |
                     PWM_X_GENB_ACTCMPBD_ZERO);
    }

}

void pwmClockSet(uint32_t clock)
{
    PWM_CC = (PWM_CC & ~(PWM_CC_USEPWM | PWM_CC_PWMDIV_M)) | clock;
}

void pwmGenPeriodSet(uint32_t period)
{
    if (PWM2_CTL & PWM_X_CTL_MODE)
    {
        // Se o contador estiver no modo up/down, o valor de LOAD será metade
        // do período (UP + DOWN = LOAD).
        if ((period / 2) < 65536)
            PWM2_LOAD = period / 2;
    }
    else
    {
        // Se o contador estiver no modo down, apenas transformamos
        // para um número possível dentro de 16 bits.
        if ((period <= 65536) && (period != 0))
            PWM2_LOAD = period - 1;
    }
}

void pwmGenEnable()
{
    PWM2_CTL |= PWM_X_CTL_ENABLE;
}

void pwmGenDisable()
{
    PWM2_CTL &= ~(PWM_X_CTL_ENABLE);
}

void pwmPulseWidthSet(uint32_t ui32PWMOut, uint32_t pulseWidth)
{
    uint32_t period, cmpValue;


    // Se o contador estiver em up/down, divide a largura por 2.
    if (PWM2_CTL & PWM_X_CTL_MODE)
        pulseWidth /= 2;

    // Get the period.
    period = PWM2_LOAD;

    // Se a largura for maior que o período,
    // ela não vai mudar.
    if (pulseWidth >= period)
        return;

    cmpValue = period - pulseWidth;

    if (ui32PWMOut & 0x01)
        PWM2_CMPB = cmpValue;
    else
        PWM2_CMPA = cmpValue;
}

void pwmOutputEnable()
{
    PWM_ENABLE |= M0PWM5;
}

void pwmOutputDisable()
{
    PWM_ENABLE &= ~M0PWM5;
}
