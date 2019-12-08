#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
bool startNewUART = true;
void UART(void const* arg)
{
	osEvent event;
	int i;
    UART0_TxString("UART iniciada\r\n");
	while(1)
	{
		// Essa thread so executa segundo a ordem do escalonador
        // o escalonador so ativa ela quando recebe uma interrupcao na UART
        event = osSignalWait(SIG_GANTT, osWaitForever);
		if(startNewUART)
        {
            clearUART();
            UART0_TxString("gantt\r\n");
            UART0_TxString("\t title A Gantt Diagram\r\n");
            UART0_TxString("\t dateFormat DDD\r\n");
            startNewUART = false;
        }
        printGanttDiagram();
		threadYield();
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
    volatile int32_t aux;
	c = UART0_DR_R & 0xFF;
    startNewUART = true;
    UART0_TxString("Interrup\r\n");
	aux = osSignalSet(tidMain, SIG_GANTT);
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
	UART0_TxString("\r\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

}
/* Funcao: void printGanttDiagram(void)
 * Imprime na UART o diagrama de Gantt
 * Param: Nenhum
 * Ret: Nenhum
 */
void printGanttDiagram()
{
    int i,j, numberOfActivations;
    char buffer[64];
    Gantt_Info currentInfo;
    for(i=0; i < TOTAL_MATH_THREADS; i++)
    {
        numberOfActivations = 0;
        currentInfo = ganttInfo[i];
        sprintf(buffer, "section thread%c", currentInfo.charId);
        UART0_TxString(buffer);
        UART0_TxString("\r\nTask: ");
        for(j = 0; j < 4096; j++)
        {
            if(currentInfo.ganttString[j] != ':')
            {
                UART0_TxChar(currentInfo.ganttString[j]);
            }
            if(currentInfo.ganttString[j] == ':')
            {
                numberOfActivations++;
                UART0_TxString("\r\n");
                if(numberOfActivations == currentInfo.activations || currentInfo.activations == 0)
                {
                    break;
                }
                else
                {
                    UART0_TxString("Task: ");
                }
            }
            
        }
    }
    
    //UART0_TxString(info.ganttString[i]);
//    for(i = 0; i<4096;i++)
//    {
//        while(info.ganttString[i] != ':')
//        {
//            UART0_TxChar(info.ganttString[i]);
//        }
//        if(info.ganttString[i] == ':') // indicador de novo par de ticks
//        {
//            numberOfActivations++;
//            UART0_TxString("\r\n");
//        }
//        if(numberOfActivations == info.activations) // O resto da string é lixo
//        {
//            break;
//        }
//    }
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