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

typedef struct{
	uint16_t ids[TOTAL_MATH_THREADS];
	uint32_t durations[TOTAL_MATH_THREADS];
}Gantt_Info;

#endif
