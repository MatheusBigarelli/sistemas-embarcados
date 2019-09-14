#ifndef __THREAD__H
#define __THREAD__H

#include "cmsis_os.h"

void initThreads(void);
// 1. Gera��o da chave de teste
void getKeyThread(void const *arg);
osThreadDef(getKeyThread, osPriorityNormal, 1, 0);
// 2. Decodifica��o de toda a mensagem
void decodeMessageThread(void const *arg);
osThreadDef(decodeMessageThread, osPriorityNormal, 1, 0);
// 3. Teste da pen�ltima word
void testNextToLastWordThread(void const *arg);
osThreadDef(testNextToLastWordThread, osPriorityNormal, 1, 0);
// 4. Teste da �ltima word
void testLastWordThread(void const *arg);
osThreadDef(testLastWordThread, osPriorityNormal, 1, 0);
// 5. Escrita do resultado no console
void printMessageThread(void const *arg);
osThreadDef(printMessageThread, osPriorityNormal, 1, 0);

#endif