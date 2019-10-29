#include "uart.h"

void UART(void const* args)
{
    #if SIMULADOR == 0
    UART0_TxString("Ola seu manezao\r\n");
    #endif
    
    while (true)
    {
        UART0_TxString("Toma no seu cu\r\n");
    }
}


void UART_init(void)
{
    // Precisa desabilitar?
    // Datasheet pag 1164
    // UART0_CTL_R  = 0;

    //Ativar o clock para porta
    SYSCTL_RCGCUART_R |= UART_ALL_PORTS;
    //Esperar porta ficar pronta
    while (SYSCTL_PRUART_R != UART_ALL_PORTS);

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
    while ((UART0_FR_R & 0x10) > 0); // FIFO vazia
    
	return (unsigned char)(UART0_DR_R & 0xFF);
}

void UART0_TxChar(unsigned char data)
{
    while ((UART0_FR_R & 0X20) > 0); // FIFO cheia

    UART0_DR_R = data;
}

void UART0_TxString(unsigned char *data)
{
    size_t size = strlen((char *)data);
    int i = 0;
    for (i = 0; i < size; i++)
        UART0_TxChar(data[i]);
}
