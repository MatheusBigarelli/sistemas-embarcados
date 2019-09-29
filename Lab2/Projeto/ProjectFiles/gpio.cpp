#include "gpio.h"

void GPIO_Init(void)
{
	//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
	SYSCTL_RCGCGPIO_R |= GPIO_ALL_PORTS;

	//1b.   após isso verificar no PRGPIO se a porta está pronta para uso.
  while((SYSCTL_PRGPIO_R & (GPIO_ALL_PORTS) ) != (GPIO_ALL_PORTS) ){};	

	// 2. Limpar o AMSEL para desabilitar a analógica
	GPIO_PORTA_AHB_AMSEL_R = 0x00;
	GPIO_PORTN_AMSEL_R = 0x00;		

	// 3. Limpar PCTL para selecionar o GPIO
	GPIO_PORTA_AHB_PCTL_R = (0x11);
	GPIO_PORTN_PCTL_R = 0x00;

	// 4. DIR para 0 se for entrada, 1 se for saída	
	GPIO_PORTN_DIR_R = GPIO_PORTN_BITS;

	// 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem função alternativa	
	GPIO_PORTA_AHB_AFSEL_R = GPIO_PORTA_BITS; 
	GPIO_PORTN_AFSEL_R = 0x00;	  

	// 6. Setar os bits de DEN para habilitar I/O digital	
	GPIO_PORTA_AHB_DEN_R = GPIO_PORTA_BITS; 
	GPIO_PORTN_DEN_R = GPIO_PORTN_BITS;
}

void PortN_Output(uint32_t value)
{
	uint32_t temp;
	
	//vamos zerar somente os bits que podem ser alterados
	temp = GPIO_PORTN_DATA_R & ~GPIO_PORTN_BITS;

	//agora vamos fazer o OR com o valor recebido na função
	GPIO_PORTN_DATA_R = temp | value; 
}