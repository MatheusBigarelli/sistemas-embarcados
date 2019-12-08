#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cmsis_os.h"

#include "TM4C129.h" // Device header

#define SIMULADOR 0
#define SIG_GANTT 0x00000020
#define TOTAL_MATH_THREADS 6
#define TOTAL_THREADS TOTAL_MATH_THREADS + 1




typedef enum{
	WAITING = 0,
	READY = 1,
	RUNNING = 2
}STATE;

typedef enum{
    THREAD_A_INDEX = 0,
    THREAD_B_INDEX,
    THREAD_C_INDEX,
    THREAD_D_INDEX,
    THREAD_E_INDEX,
    THREAD_F_INDEX,
    
}THREAD_INDEX;

typedef struct{
	uint32_t startTick, endTick;
    char charId;
    char ganttString[4096];//Armazena startTick 0,endTick 0:startTick 1,endTick 1:...
    uint32_t activations;
}Gantt_Info;

typedef struct{
    osThreadId id;
    char charId;
	int32_t staticPriority;
    uint32_t startTick;
	uint32_t laxityTimeInTicks;
	STATE currentState;
	double executionPercent;
    double deadlinePercent;
	uint32_t delayInTicks;
	uint32_t durationInTicks;
    uint32_t tickOfDeadline;
    bool isRealtime;
}Thread_Info;

extern Thread_Info threadsInfo[TOTAL_THREADS];
extern Gantt_Info ganttInfo[TOTAL_THREADS];

extern void fillGantInfo(THREAD_INDEX tindex, uint32_t startTick, uint32_t endTick, char charId);




#endif
