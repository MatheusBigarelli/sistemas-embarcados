#include "gpio.h"

void GPIO_Init(void)
{
    //1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
    SYSCTL_RCGCGPIO_R |= GPIO_ALL_PORTS;
    
    //1b.   ap�s isso verificar no PRGPIO se a porta est� pronta para uso.
    while ((SYSCTL_PRGPIO_R & (GPIO_ALL_PORTS)) != (GPIO_ALL_PORTS));

    // 2. Limpar o AMSEL para desabilitar a anal�gica
    GPIO_PORTA_AHB_AMSEL_R = 0x00;

    // 3. Limpar PCTL para selecionar o GPIO
    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R & 0xFFFFFF00) + 0x11;

    // 4. DIR para 0 se for entrada, 1 se for sa�da

    // 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem fun��o alternativa
    GPIO_PORTA_AHB_AFSEL_R = GPIO_PORTA_BITS;
    
	// 6. Setar os bits de DEN para habilitar I/O digital
    GPIO_PORTA_AHB_DEN_R = GPIO_PORTA_BITS;
}



