#include "uart.h"
#include <string.h>

/* Funcao: void UART(void const* args)
 * Thread UART
 * Param: Nenhum
 * Ret: Nenhum
 */
ID currentMenu = MainMenu;
bool updateMenu = true;
bool parameterChanged = false;
void UART(void const* args)
{
	UART0_TxString("Digite o numero e pressione ENTER\r\n");
    while (true)
    {
		UART0_PrintMenu(currentMenu);
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
/* Variaveis utilizadas pelo handler da UART */
// Buffer para armazenar o que foi recebido na UART
char buffer[16] = "";
// Indice para o buffer do receptor da UART, deve ser declarado externamente, pois o buffer pode ser preenchido(incrementalmente) em diferentes chamadas do UART0_Handler
int i = 0;
void resetBuffer(void)
{
	int k;
	for(k = 0; k < 16; k++) // limpa o buffer
	{
		buffer[k] = 0;
	}
	
	i = 0; // Reinicia o indice do buffer
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
	
	if(i == 16) //Buffer cheio
	{
		i = 0;
	}
	if(c == '\r') // Enter pressionado
	{
		//buffer contem um comando
		if(currentMenu == MainMenu) // O comando eh de mudanca para um sub-menu
		{
			/*Esse comando deve ser do tipo <1 numeros + ENTER>
			 * Se forem digitados varias teclas elas serao ignoradas e apenas a ultima (antes do ENTER) sera considerada
			 */
			currentMenu = (ID)(buffer[i-1] - '0'); // '1' - '0' em decimal = 49 - 48 = 1, '2' - '0' = 50 - 48 = 2 ...
			//currentMenu = GanttMenu;
			updateMenu = true;  //Sinaliza que menu deve ser atualizado
			UART0_PrintMenu(currentMenu);
		}
		else // O comando configurou um parametro(frequencia, amplitude ou mostrar Gantt), agora deve voltar para menu principal
		{
			currentMenu = MainMenu; // Volta para menu principal
			updateMenu = true; //Sinaliza que menu deve ser atualizado
			parameterChanged = true; // Sinaliza que um parametro de configuracao foi alterado(possivelmente)
			for(k = 0; k < i;k++)
			{
				newParameterStr[k] = buffer[k];
			}
			UART0_TxString(newParameterStr);
			UART0_PrintMenu(currentMenu);
		}
		resetBuffer(); //Necessario?
	}
	else
	{
		buffer[i] = c;
		i++;
	}
}

/* Funcao: void clearUART(void)
 * Limpa o terminal onde a UART esta enviando dados.
 * Param: Nenhum
 * Ret: Nenhum
 */
void clearUART(void)
{
//		UART0_TxChar(27);
//		UART0_TxChar(12);
//        // Os comandos anteriores removendo o proximo caractere(por que?)
//		UART0_TxChar('1'); // Escreve 1 apenas para corrigir o comportamento acima
	UART0_TxString("\r\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

}
/* Funcao: void UART0_PrintMenu(uint8_t menuID)
 * Imprime na UART um menu, de acordo com o parametro
 * Param: menuID -> Indica qual menu deve ser apresentado
 * Ret: Nenhum
 */
void UART0_PrintMenu(ID menuID)
{
	int k;
	if(updateMenu == false)
	{
		return;
	}
    clearUART();
	if(parameterChanged)
	{
		parameterChanged = false;
		UART0_TxString("Valor configurado: ");
		UART0_TxString(buffer);
		UART0_TxString("\r\n");
	}
    switch (menuID) {
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
        UART0_TxString("3)Dente-de-serra\r\n");
        UART0_TxString("4)Quadrada\r\n");
        UART0_TxString("5)Trapezoidal\r\n");
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
        UART0_TxString("Como\r\n");
    }
	updateMenu = false; // Menu foi atualizado, desativa flag de atualizacao
}
/* Funcao: void UART0_TxChar(char data)
 * Envia para UART um caractere
 * Param: data -> Caractere a ser enviado
 * Ret: Nenhum
 */
void UART0_TxChar(char data)
{
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
    for (i = 0; i < size; i++)
	{
       UART0_TxChar(data[i]);
	}

}
