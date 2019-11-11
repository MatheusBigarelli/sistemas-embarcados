#include "mypwm.h"


// Baseado no exemplo de configuracao do datasheet (pagina 1678)
void PWM_Init(void)
{
    SYSCTL_RCGCPWM_R |= 0x1;    // Ativando clock no PWM. 
    SYSCTL_RCGCGPIO_R |= GPIO_RCGC_PORTG;   // Ativando clock na porta G.
    while ((SYSCTL_PRGPIO_R & (GPIO_RCGC_PORTG)) != (GPIO_RCGC_PORTG))  // Espera até o clock estar
    {                                                                   // devidamente ativado.
    };

    // Config do pino G1
    GPIO_PORTG_AHB_AMSEL_R = 0x00;          // Desativa funcao analogica
    GPIO_PORTG_AHB_PCTL_R = 0x60;           // Para PWM o valor eh 6, como estamos no pino 1
                                            // (e cada pino tem 4bits no registrador) devemos colocar 0110 0000
    GPIO_PORTG_AHB_DIR_R = GPIO_PORTG_BITS; // Define pinos como saida

    GPIO_PORTG_AHB_AFSEL_R = GPIO_PORTG_BITS; // Sinaliza que os pinos serao usados com funcao alternativa. O pino G1 alternativamente funciona comoM0PWM5
    GPIO_PORTG_AHB_DEN_R = GPIO_PORTG_BITS;   // Ativa os pinos

    PWM0_CC_R = PWM_SYSCLK_DIV2;    // Divide por 2 o clock do sistema (Ex.: 120/2=60Mhz)
    PWM0_2_CTL_R = ~PWM_X_ENABLE;    // Desabilita a saída do gerador.
    PWM0_2_GENB_R = 0x80c;    // Seta a opção de @TODO encontrar opção no datasheet

    PWM_Enable();
}

void setPeriod(uint16_t period)
{
	
    if (PWM_PERIOD_MIN < period && period < PWM_PERIOD_MAX)
        PWM0_2_LOAD_R = 60*period; // Usando o modo count down
}

void setDutyCycle(float dutyCycle)
{
    // tick_alto = Total(1 - dutycycle)
    // ticks_alto = 4000*(1-0.75)
    // ticks_alto = 4000*0.25 = 1000
    // Ou seja quando o contador chegar em 1000 o PWM vai para alto e quando chegar em 4000 volta para baixo
    // Logo ele fica de 1000 ate 4000 em alto(75% de duty cycle)
    PWM0_2_CMPB_R = PWM0_2_LOAD_R * (1 - dutyCycle);
}

void PWM_Enable(void)
{
    PWM0_2_CTL_R = PWM_X_ENABLE;       // Ativa o bloco PWM conforme pagina 1712
    PWM0_ENABLE_R = PWM0_ENABLE_M0PWM5; // Ativa M0PWM5 conforme pagina 1686
}

void PWM_Disable(void)
{
    PWM0_2_CTL_R = ~PWM_X_ENABLE;    // Desativa o bloco PWM conforme página 1712.
    PWM0_ENABLE_R = 0x00;    // Desativa M0PWM5 conforme página 1686.
}

