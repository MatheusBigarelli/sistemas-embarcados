/*============================================================================
 *                    Exemplos de utiliza��o do Kit
 *              EK-TM4C1294XL + Educational BooterPack MKII 
 *---------------------------------------------------------------------------*
 *                    Prof. Andr� Schneider de Oliveira
 *            Universidade Tecnol�gica Federal do Paran� (UTFPR)
 *===========================================================================
 * Autores das bibliotecas:
 * 		Allan Patrick de Souza - <allansouza@alunos.utfpr.edu.br>
 * 		Guilherme Jacichen     - <jacichen@alunos.utfpr.edu.br>
 * 		Jessica Isoton Sampaio - <jessicasampaio@alunos.utfpr.edu.br>
 * 		Mariana Carri�o        - <mcarriao@alunos.utfpr.edu.br>
 *===========================================================================*/

#include "grlib/grlib.h"

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

#include "cfaf128x128x16.h"


//To print on the screen
tContext sContext;

/*----------------------------------------------------------------------------
 *    Initializations
 *---------------------------------------------------------------------------*/

void init_all(){
	cfaf128x128x16Init();
}

void init_sidelong_menu(){
	uint8_t i;
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	
	//Escreve menu lateral:
	GrStringDraw(&sContext,"Exemplo EK-TM4C1294XL", -1, 0, (sContext.psFont->ui8Height+2)*0, true);
	GrStringDraw(&sContext,"---------------------", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"RGB", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"ACC", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"TMP", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"OPT", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"MIC", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"JOY", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"BUT", -1, 0, (sContext.psFont->ui8Height+2)*8, true);

}


#define SIMULADOR 1



/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {
	osKernelInitialize();

	#if SIMULADOR == 0
	init_all();
	init_sidelong_menu();
	#endif

	createThreads();

	osKernelStart();

	tidMain = osGetThreadId();

	return 0;
}
