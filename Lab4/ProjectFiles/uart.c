#include "uart.h"
#include <string.h>
#include <stdlib.h>

/* Funcao: void UART(void const* args)
 * Thread UART
 * Param: Nenhum
 * Ret: Nenhum
 */

void UART(const void* args)
{
	bool flag = false;
	osStatus  status;
	osEvent evt;
	char receveidChar = 'h';
	char buffer[32];
	int currentIndex = 0;
	SignalConfig_t* config;
//	UART0_TxString("Digite o numero e pressione ENTER\r\n");

	ID currentMenu = MainMenu;
	bool updateMenu = true;
	osMessagePut(qidUARTMsgBox, (uint8_t)receveidChar, osWaitForever);
	UART0_PrintMenu(currentMenu);
    while (true)
    {		
		evt = osMessageGet(qidUARTMsgBox, osWaitForever);
		if(evt.status == osEventMessage)
		{
			receveidChar = (char)evt.value.p;
			if( checkChar(receveidChar) )
			{
				buffer[currentIndex] = receveidChar;
				currentIndex++;
			}
			if( isCommand(buffer, currentIndex) )
			{
				if(currentMenu == MainMenu) // O comando eh de mudanca para sub-menu (pois esta no menu principal)
				{
					currentMenu = (ID)(buffer[currentIndex-1 - 1] - '0'); // '1' - '0' em decimal = 49 - 48 = 1, '2' - '0' = 50 - 48 = 2 ...
				} else { // O comando configurou um parametro(frequencia, amplitude ou mostrar Gantt), agora deve voltar para menu principal
					switch (currentMenu)
					{
						case WaveformMenu:
							// Mandando info para thread do display
							config = osMailAlloc(qidDisplayMailQueue, osWaitForever);
							config->waveform = buffer[0] - '1'; // No menu a primeira onda é índice 1.
							config->changedParameter = WAVEFORM;
							osMailPut(qidDisplayMailQueue, config);
							// Mandando info para thread do gerador de sinal.
							config = osMailAlloc(qidSigGenMailQueue, osWaitForever);
							config->waveform = buffer[0] - '1'; // No menu a primeira onda é índice 1.
							config->changedParameter = WAVEFORM;
							osMailPut(qidSigGenMailQueue, config);
							break;
						case FreqMenu:
							// Mandando info para thread do display.
							config = osMailAlloc(qidDisplayMailQueue, osWaitForever);
							config->frequency = extractNumberFromCommand(buffer, currentIndex, FreqMenu);
							config->changedParameter = FREQUENCY;
							osMailPut(qidDisplayMailQueue, config);
							// Mandando info para thread do gerador de sinal.
							config = osMailAlloc(qidSigGenMailQueue, osWaitForever);
							config->frequency = extractNumberFromCommand(buffer, currentIndex, FreqMenu);
							config->changedParameter = FREQUENCY;
							osMailPut(qidSigGenMailQueue, config);
							break;
						case AmpMenu:
							// Mandando info para thread do display.
							config = osMailAlloc(qidDisplayMailQueue, osWaitForever);
							config->amplitude = extractNumberFromCommand(buffer, currentIndex, AmpMenu);
							config->changedParameter = AMPLITUDE;
							osMailPut(qidDisplayMailQueue, config);
							// Mandando info para thread do gerador de sinal.
							config = osMailAlloc(qidSigGenMailQueue, osWaitForever);
							config->amplitude = extractNumberFromCommand(buffer, currentIndex, AmpMenu);
							config->changedParameter = AMPLITUDE;
							osMailPut(qidSigGenMailQueue, config);
							break;
						default:
					}
					currentMenu = MainMenu;
				}
				clearBuffer(buffer, &currentIndex); //currentIndex eh passado por referencia para ser zerado dentro da funcao tambem
				UART0_PrintMenu(currentMenu);
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
    UART0_IBRD_R = 260;
    UART0_FBRD_R = 27;

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
void UART0_Handler(void) //Do modo como foi configurado esse handler sera chamado a cada byte recebido na UART
{
	char c;
	int k;
	char newParameterStr[16];
	c = UART0_DR_R & 0xFF;
	UART0_TxChar(c);
	osMessagePut(qidUARTMsgBox, (uint8_t)c, 0);
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
/* Funcao: void UART0_PrintMenu(uint8_t menuID)
 * Imprime na UART um menu, de acordo com o parametro
 * Param: menuID -> Indica qual menu deve ser apresentado
 * Ret: Nenhum
 */
void UART0_PrintMenu(ID currentMenu)
{
	clearUART();

	switch (currentMenu) {
		case MainMenu:
		UART0_TxString("****Menu Principal****\r\n");
		UART0_TxString("1)Mudar forma de onda\r\n");
		UART0_TxString("2)Mudar frequencia\r\n");
		UART0_TxString("3)Mudar amplitude\r\n");
		UART0_TxString("4)Imprimir Gantt\r\n");
		break;
		case WaveformMenu:
		UART0_TxString("****Menu Forma de Onda****\r\n");
		UART0_TxString("1)Senoidal\r\n");
		UART0_TxString("2)Triangular\r\n");
		UART0_TxString("3)Trapezoidal\r\n");
		UART0_TxString("4)Quadrada\r\n");
		UART0_TxString("5)Dente-de-serra\r\n");
		break;
		case FreqMenu:
		UART0_TxString("****Menu Frequencia****\r\n");
		UART0_TxString("Informe f [0-200Hz]\r\n");
		break;
		case AmpMenu:
		UART0_TxString("****Menu Amplitude****\r\n");
		UART0_TxString("Informe A [0-33V]\r\n");
		UART0_TxString("A amplitude sera dividida por 10.\r\n");
		break;
		case GanttMenu:
		UART0_TxString("****Menu Gantt****\r\n");
		UART0_TxString("1)Voltar\r\n");
		break;
		default:
		UART0_TxString("Nao devia estar aqui\r\n");
	}
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

/* Funcao: bool checkChar(char c)
 * Verifica se o caractere c eh valido (uma numero ou tecla ENTER)
 * Param: c -> caractere sendo testado
 * Ret: true se for valido, false caso contrario
 */
bool checkChar(char c)
{
	if(c == '\r'){ // Tecla ENTER
		return true;
	}
	if(c < 48 || c > 57){ //Numeros estao entre 48 e 57, qualquer coisa fora desse intervalo nao eh valido
		return false;
	}
	return true;
}

/* Funcao: bool isCommand(char *buffer, int size)
 * Verifica se o buffer contem um comando valido, baseado no menu em que o usuario estao 
 * Param: buffer -> string possivelmente contendo um comando
		  size -> tamanho do buffer
 * Ret: true se for comando valido, false caso contrario
 */
bool isCommand(char *buffer, int size)
{
	if(size < 2){ // Comando deve ter pelo menos 2 teclas (1 numero + ENTER)
		return false;
	}
	if(buffer[size-1] != '\r'){ // Um comando deve terminar em ENTER
		return false;
	}
	return true;
}

/* Funcao: void clearBuffer(char *buffer, int *size)
 * Limpa o buffer preenchendo com (int)0, nao o caractere '0'
 * Param: buffer -> buffer para ser zerado
		  size -> endereco para tamanho do buffer (sera zerado dentro da funcao tambem)
 * Ret: Nenhum
 */
void clearBuffer(char *buffer, int *size)
{
	int k;
	for(k = 0; k < *size; k++) // limpa o buffer
	{
		buffer[k] = 0;
	}
	*size = 0;
}

float extractNumberFromCommand(char *buffer, int size, ID menu)
{
	int i;
	char aux[4];
	int numberOfDigits = size - 1; // Descontando o ENTER do buffer o restante sao numeros.
	if (numberOfDigits > 2 && menu == AmpMenu)
	{ // A amplitude vai no maximo ate 33(ou seja, 2 digitos), lembrando que o usuario digita 33, mas eh configura 3.3V
		return 3.3;
	}
	if (numberOfDigits > 3 && menu == FreqMenu)
	{ // A frequencia vai no maximo ate 200(ou seja, 3 digitos)
		return 200;
	}
	for (i = 0; i < numberOfDigits; i++)
	{
		aux[i] = buffer[i];
	}
	if (menu == AmpMenu)
	{
		return atof(aux) / 10.0;
	}
	if (menu == FreqMenu)
	{
		return atof(aux);
	}
}
