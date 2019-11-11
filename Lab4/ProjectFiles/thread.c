#include "thread.h"


GanttInfo_t activationSeq[MAX_ACTIVATIONS];
uint32_t currentActivation = 0;
bool activationSequenceFull = false;

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

// Usado para gerar o gantt
osThreadId previousId;
void tickCounter(uint32_t* previousTick, char* name)
{
	uint32_t tick = osKernelSysTick();
	uint32_t quantum = osKernelSysTickMicroSec(5000); // Quantum eh de 5ms(5000us)
	if(tick - *previousTick >= quantum){
		if(osThreadGetId() == previousId){
			currentActivation--;
		}
		strcpy(activationSeq[currentActivation].name, name);
		activationSeq[currentActivation].lastTick = (uint16_t)(1000*(*previousTick/(double)osKernelSysTickFrequency))%1000;
		currentActivation++;	
		if(currentActivation == MAX_ACTIVATIONS){
			currentActivation = 0;
			activationSequenceFull = true;
		}
		previousId = osThreadGetId();
	}
	*previousTick = tick;
}
