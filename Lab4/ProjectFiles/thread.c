#include "thread.h"

osThreadDef(UART, osPriorityNormal, 1, 0);
osThreadId tidUART;

osThreadDef(Display, osPriorityNormal, 1, 0);
osThreadId tidDisplay;

osThreadDef(SignalGenerator, osPriorityNormal, 1, 0);
osThreadId tidSignalGenerator;

osMessageQDef(uartMsgBox, 1, char);
osMessageQId qidUARTMsgBox;

osMailQDef(sigGenMailQ, MAX_SIG_CFG_MAILS, SignalConfig_t);
osMailQId qidSigGenMailQueue;

osMailQDef(displayMailQ, MAX_SIG_CFG_MAILS, SignalConfig_t);
osMailQId qidDisplayMailQueue;

void createThreads(void)
{
    tidUART = osThreadCreate(osThread(UART), NULL);
    #if SIMULADOR == 0

    tidSignalGenerator = osThreadCreate(osThread(SignalGenerator), NULL);
    osThreadCreate(osThread(Display), NULL);
    #endif
}

void createMailQueue(void)
{
    qidUARTMsgBox = osMessageCreate(osMessageQ(uartMsgBox), NULL);
    qidSigGenMailQueue = osMailCreate(osMailQ(sigGenMailQ), NULL);
    qidDisplayMailQueue = osMailCreate(osMailQ(displayMailQ), NULL);
}