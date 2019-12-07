#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cmsis_os.h"

#include "TM4C129.h" // Device header

#define SIMULADOR 1
#define SIG_GANTT 0x00000020
#define TOTAL_MATH_THREADS 6
#define TOTAL_THREADS TOTAL_MATH_THREADS + 1


typedef struct{
	uint16_t ids[TOTAL_MATH_THREADS];
	uint32_t durations[TOTAL_MATH_THREADS];
}Gantt_Info;

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
    osThreadId id;
    char charId;
	int32_t dinamicPriority, staticPriority;
	uint32_t laxityTimeInTicks;
	STATE currentState;
	double executionPercent;
	uint32_t delayInTicks;
	uint32_t durationInTicks;
    uint32_t tickOfDeadline;
    uint16_t maxIterationsPerCycle;
}Thread_Info;


extern Thread_Info threadsInfo[TOTAL_THREADS];

#endif
