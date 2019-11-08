#include "mypwm.h"


// Baseado no exemplo de configuracao do datasheet (pagina 1678)
void PWM_Init()
{
    SYSCTL_RCGCPWM_R |= 0x1;
    SYSCTL_RCGCGPIO_R |= GPIO_RCGC_PORTG;
    while ((SYSCTL_PRGPIO_R & (GPIO_RCGC_PORTG)) != (GPIO_RCGC_PORTG))
    {
    };

    // Config do pino G1
    GPIO_PORTG_AHB_AMSEL_R = 0x00;          // Desativa funcao analogica
    GPIO_PORTG_AHB_PCTL_R = 0x60;           // Para PWM o valor eh 6, como estamos no pino 1(e cada pino tem 4bits no registrador) devemos colocar 0110 0000
    GPIO_PORTG_AHB_DIR_R = GPIO_PORTG_BITS; // Define pinos como saida

    GPIO_PORTG_AHB_AFSEL_R = GPIO_PORTG_BITS; // Sinaliza que os pinos serao usados com funcao alternativa. O pino G1 alternativamente funciona comoM0PWM5
    GPIO_PORTG_AHB_DEN_R = GPIO_PORTG_BITS;   // Ativa os pinos

    PWM0_CC_R = 0x100; // Divide por 2 o clock do sistema (Ex.: 80/2=40Mhz)
    PWM0_2_CTL_R = 0x0;
    PWM0_2_GENB_R = 0x80c;

    PWM_Enable();
}

void setPeriod(uint16_t period)
{
    if (0 < period && period < 65536)
        PWM0_2_LOAD_R = period; // Usando o modo count down
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

void PWM_Enable()
{
    PWM0_2_CTL_R = 0x1;       // Ativa o bloco PWM conforme pagina 1712
    PWM0_ENABLE_R = 0x1 << 5; // Ativa M0PWM5 conforme pagina 1686
}

