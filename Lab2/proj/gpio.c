#include "gpio.h"

// Ativando o port A para comunicação UART
void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R |= GPIO_ALL_PORTS;
	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_ALL_PORTS) ) != (GPIO_ALL_PORTS) ){};
	
	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTA_AHB_AMSEL_R = 0x00;
		
	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R&0xFFFFFF00) + 0x11;
		
		
	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTA_AHB_AFSEL_R = GPIO_PORTA_BITS;
	  
	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTA_AHB_DEN_R = GPIO_PORTA_BITS; 

}
