#include "display.h"

extern tContext sContext;

void Display(void const* args)
{
    osEvent event;
    Display_Info *info;

    displayInit();
    printHeader();
    threadYield();

    while (true)
    {
        event = osMailGet(qidDisplayMailQueue, osWaitForever);
        if (event.status == osEventMail)
        {
            info = (Display_Info *)event.value.p;
            if (info != NULL)
            {
                updateInfo(info);
                osMailFree(qidDisplayMailQueue, info);
            }
        }
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
    GrStringDraw(&sContext, " |  P   T  S % D Q F ", -1, 0, (sContext.psFont->ui8Height+3)*0, true);
	GrStringDraw(&sContext,"-+-------------------", -1, 0, (sContext.psFont->ui8Height+3)*1, true);
    GrStringDraw(&sContext, "A|", -1, 0, (sContext.psFont->ui8Height+3)*2, true);
    GrStringDraw(&sContext, "B|", -1, 0, (sContext.psFont->ui8Height+3)*3, true);
    GrStringDraw(&sContext, "C|", -1, 0, (sContext.psFont->ui8Height+3)*4, true);
    GrStringDraw(&sContext, "D|", -1, 0, (sContext.psFont->ui8Height+3)*5, true);
    GrStringDraw(&sContext, "E|", -1, 0, (sContext.psFont->ui8Height+3)*6, true);
    GrStringDraw(&sContext, "F|", -1, 0, (sContext.psFont->ui8Height+3)*7, true);
}

void updateInfo(Display_Info *info)
{
    char buf[NUM_DIGITS];
    uint8_t index = info->charId - 'A';
    sprintf(buf, "%4x", info->staticPriority);
    GrStringDraw(&sContext, buf, -1, 2 * (sContext.psFont->ui8MaxWidth),
        (sContext.psFont->ui8Height + 3) * (2 + index), true);

    sprintf(buf, "%3x", info->laxityTimeInTicks);
    GrStringDraw(&sContext, buf, -1, 5 * (sContext.psFont->ui8MaxWidth),
        (sContext.psFont->ui8Height + 3) * (2 + index), true);

    sprintf(buf, "%1x", info->currentState);
    GrStringDraw(&sContext, buf, -1, 10 * (sContext.psFont->ui8MaxWidth),
        (sContext.psFont->ui8Height + 3) * (2 + index), true);

    sprintf(buf, "%2x", info->executionPercent);
    GrStringDraw(&sContext, buf, -1, 11 * (sContext.psFont->ui8MaxWidth),
        (sContext.psFont->ui8Height + 3) * (2 + index), true);

    sprintf(buf, "%3x", info->delayInTicks);
    GrStringDraw(&sContext, buf, -1, 14 * (sContext.psFont->ui8MaxWidth),
        (sContext.psFont->ui8Height + 3) * (2 + index), true);

    sprintf(buf, "%3x", info->delayInTicks);
    GrStringDraw(&sContext, buf, -1, 14 * (sContext.psFont->ui8MaxWidth),
        (sContext.psFont->ui8Height + 3) * (2 + index), true);

    sprintf(buf, "%1x", info->delayInTicks);
    GrStringDraw(&sContext, buf, -1, 14 * (sContext.psFont->ui8MaxWidth),
        (sContext.psFont->ui8Height + 3) * (2 + index), true);

    sprintf(buf, "%2x", info->delayInTicks);
    GrStringDraw(&sContext, buf, -1, 14 * (sContext.psFont->ui8MaxWidth),
        (sContext.psFont->ui8Height + 3) * (2 + index), true);
}
