#include "uart.h"

//Inicializa UART, nesse caso apenas UART0 (conectada na interface usb)
void UART_Init(void)
{
	//Ativar o clock para porta
	SYSCTL_RCGCUART_R |= UART_ALL_PORTS;
	//Esperar porta ficar pronta 
	while(SYSCTL_PRUART_R != UART_ALL_PORTS);
	
	//Desabilita UART para configurar
	UART0_CTL_R = 0x0;
	
	//Escrever Baud-Rate para 19200 bps
	UART0_IBRD_R = 260;
	UART0_FBRD_R = 27;
	
	//Configurando UARTLCRH
	UART0_LCRH_R |= 0x70; // 8bits, Ativa FIFO, 1 stop bit, sem paridade
	
	//Garantir mesmo clock para UART e sistema
	UART0_CC_R = 0x0;
	
	//Habilitar RXE, TXE e UARTEN
	UART0_CTL_R |= 0x301;
	
}


unsigned char UART0_RxChar(void)
{
	while((UART0_FR_R & 0x10) > 0); // FIFO vazia
	return (unsigned char)(UART0_DR_R & 0xFF);
}

void UART0_TxChar(const char data)
{
	while((UART0_FR_R & 0X20) > 0); // FIFO cheia
	UART0_DR_R = data;
}

void UART0_TxString(const char* data)
{
	while(*data)
	{
		UART0_TxChar(*(data++));
		
	}
}