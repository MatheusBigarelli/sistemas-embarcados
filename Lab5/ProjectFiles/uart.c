#include "uart.h"
#include <string.h>
#include <stdlib.h>
/* Funcao: void GPIO_Init(void)
 * Inicializa o GPIO para UART
 * Param: Nenhum
 * Ret: Nenhum
 */
void GPIO_Init(void)
{
		//1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO
    SYSCTL_RCGCGPIO_R |= GPIO_ALL_PORTS;
    
    //1b.   ap?s isso verificar no PRGPIO se a porta esta pronta para uso.
    while ((SYSCTL_PRGPIO_R & (GPIO_ALL_PORTS)) != (GPIO_ALL_PORTS));

    // 2. Limpar o AMSEL para desabilitar a analogica
    GPIO_PORTA_AHB_AMSEL_R = 0x00;

    // 3. Limpar PCTL para selecionar o GPIO
    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R & 0xFFFFFF00) + 0x11;

    // 4. DIR para 0 se for entrada, 1 se for saida

    // 5. Limpar os bits AFSEL para 0 para selecionar GPIO sem funcao alternativa
    GPIO_PORTA_AHB_AFSEL_R = GPIO_PORTA_BITS;
    
		// 6. Setar os bits de DEN para habilitar I/O digital
    GPIO_PORTA_AHB_DEN_R = GPIO_PORTA_BITS;
}

void UART(void const* arg)
{
	osEvent event;
	Gantt_Info* info;
	while(1)
	{
		
		event = osMailGet(qidUartMailQueue, osWaitForever);
		if(event.status == osEventMail){
			info = (Gantt_Info*)event.value.p;
			if(info != NULL){
				clearUART();
				printGanttDiagram();
				osMailFree(qidUartMailQueue, info);
			}
		}
		
	}
}

/* Funcao: void UART_init(void)
 * Inicializa o periferico UART
 * Param: Nenhum
 * Ret: Nenhum
 */
void UART_init(void)
{
    //Ativar o clock para porta
    SYSCTL_RCGCUART_R |= UART_ALL_PORTS;
    //Esperar porta ficar pronta
    while (SYSCTL_PRUART_R != UART_ALL_PORTS);

    //Desabilita UART para configurar
    UART0_CTL_R = 0x0;

    //Escrever Baud-Rate para 19200 bps
		// BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
		// BRD = BRDI + BRDF = 80 000 000 / (16 * 19200)
		// 260 + 0.41
		// UART0_IBRD_R = 260;
    // UART0_FBRD_R = integer( 0.41 * 64 + 0.5) =integer(27,16) = 27 ;
		// Para clock = 120MHz
		// 390 + 0.625
		// UART0_IBRD_R = 390;
    // UART0_FBRD_R = integer( 0.625 * 64 + 0.5) =integer(40,5) = 40 ;
		UART0_IBRD_R = 390;
    UART0_FBRD_R = 40 ;

    //Configurando UARTLCRH
    UART0_LCRH_R |= 0x60; // 8bits, Ativa FIFO, 1 stop bit, sem paridade

    //Garantir mesmo clock para UART e sistema
    UART0_CC_R = 0x0;

		//Conforme especificado na pagina 1169 do datasheet a UART tem 2 filas de tamanho 16(elementos sao do tipo char), uma para transmitir e outra para receber
    //Por padrao a interrupcao no receptor eh ativada apos receber 8bytes
    // Para alterar deve escrever no registrador UARTIFLS, de acordo com a pagina 1192 do datasheet
    //UART0_IFLS_R = 0x2;

    //Habilitar RXE, TXE e UARTEN
    UART0_CTL_R |= 0x301;


    //Interrupcao Numero de interrupcao da UART0  eh 5
    UART0_IM_R = 0x1 << 4; // Ativa apenas interrupcao para recebimento (Pagina 1196 do datasheet)
    UART0_RIS_R = 0x1 << 4; // Ativa apenas interrupcao para recebimento (Pagina 1200 do datasheet)
    NVIC_PRI1_R = 0x0 << 13; // Seta prioridade 0 para interrupcao no bit 13 dentro do registrador PRI1 (Pagina 159 do datasheet)
    NVIC_EN0_R = 0x1 << 5; // Ativa o 5 bit no primeiro registrador de Enable do NVIC (Pagina 154 do datasheet)
}
/* Funcao: void UART0_Handler(void)
 * Handler da interrupcao do receptor da UART
 * Param: Nenhum
 * Ret: Nenhum
 */
void UART0_Handler(void) //Do modo como foi configurado esse handler sera chamado a cada byte(char) recebido na UART
{
	char c;
	int k;
	char newParameterStr[16];
	c = UART0_DR_R & 0xFF;
	osSignalSet(tidMain, SIG_GANTT);
}

/* Funcao: void clearUART(void)
 * Limpa o terminal onde a UART esta enviando dados.
 * Param: Nenhum
 * Ret: Nenhum
 */
void clearUART(void)
{
	UART0_TxChar(27);
	UART0_TxChar(12);
	// Os comandos anteriores removendo o proximo caractere(por que?)
	UART0_TxChar('1'); // Escreve 1 apenas para corrigir o comportamento acima
	//UART0_TxString("\r\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

}
/* Funcao: void printGanttDiagram(void)
 * Imprime na UART o diagrama de Gantt
 * Param: Nenhum
 * Ret: Nenhum
 */
void printGanttDiagram(void)
{
    int i;
	UART0_TxString("gantt\r\n");
	UART0_TxString("\t title A Gantt Diagram\r\n");
	UART0_TxString("\t dateFormat SSS\r\n");
	UART0_TxString("\t axisFormat %L\r\n");//Tempo em us
    for(i = 0; i<TOTAL_THREADS;i++)
    {
        
    }
	UART0_TxString("Desenvolvendo...\r\n");
	UART0_TxString("\r\n");
}
/* Funcao: void UART0_TxChar(char data)
 * Envia para UART um caractere
 * Param: data -> Caractere a ser enviado
 * Ret: Nenhum
 */
void UART0_TxChar(char data)
{
		if(SIMULADOR == 1){
					return;
		}
    while ((UART0_FR_R & 0X20) > 0); // FIFO cheia

    UART0_DR_R = data;
}
/* Funcao: void UART0_TxString(char *data)
 * Envia para UART uma string
 * Param: data -> String a ser enviada
 * Ret: Nenhum
 */
void UART0_TxString(char *data)
{
    size_t size = strlen((char *)data);
    int i = 0;
		if(SIMULADOR == 1){
					return;
		}
    for (i = 0; i < size; i++)
	{
       UART0_TxChar(data[i]);
	}

}