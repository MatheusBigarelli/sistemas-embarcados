/**
 * Arquivo para inclusões de definições comuns 
 * aos demais arquivos
 */
#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <stdbool.h>

#include "cmsis_os.h"

#include "TM4C129.h"


#define MAIL_INSTANT_CHECK_TIME 0 // O tempo 0 no osMailGet() retorna imediatamente.
#define MAX_SIG_CFG_MAILS 3

#define SIMULADOR 0

#define MAX_ACTIVATIONS 50

typedef enum
{
    SINUSOIDAL,
    TRIANGULAR,
    TRAPEZOIDAL,
    SQUARE,
    SAWTOOTH
} WAVEFORMS;

typedef enum
{
    WAVEFORM,
    FREQUENCY,
    AMPLITUDE,
} PARAMETERS;

typedef struct
{
    float amplitude;
    float frequency;
    WAVEFORMS waveform;
    PARAMETERS changedParameter;
} SignalConfig_t;


typedef struct
{
	char name[16];
	double lastTick;
} GanttInfo_t;

#endif
