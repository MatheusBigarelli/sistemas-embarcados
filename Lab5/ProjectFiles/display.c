#include "display.h"

extern tContext sContext;

void Display(void const* args)
{
    displayInit();

    printHeader();

    
    threadYield();

    while (true)
    {

    }
}

void displayInit()
{
    GrContextInit(&sContext, &g_sCfaf128x128x16);

    GrFlush(&sContext);
    GrContextFontSet(&sContext, g_psFontFixed6x8);

    GrContextForegroundSet(&sContext, ClrWhite);
    GrContextBackgroundSet(&sContext, ClrBlack);
}

void printHeader()
{
    GrStringDraw(&sContext, " | A  B  C  D  E  F", -1, 0, (sContext.psFont->ui8Height+3)*0, true);
	GrStringDraw(&sContext,"-+-------------------", -1, 0, (sContext.psFont->ui8Height+3)*1, true);
    GrStringDraw(&sContext, "P|", -1, 0, (sContext.psFont->ui8Height+3)*2, true);
    GrStringDraw(&sContext, "T|", -1, 0, (sContext.psFont->ui8Height+3)*3, true);
    GrStringDraw(&sContext, "S|", -1, 0, (sContext.psFont->ui8Height+3)*4, true);
    GrStringDraw(&sContext, "%|", -1, 0, (sContext.psFont->ui8Height+3)*5, true);
    GrStringDraw(&sContext, "D|", -1, 0, (sContext.psFont->ui8Height+3)*6, true);
    GrStringDraw(&sContext, "Q|", -1, 0, (sContext.psFont->ui8Height+3)*7, true);
    GrStringDraw(&sContext, "F|", -1, 0, (sContext.psFont->ui8Height+3)*8, true);
}


