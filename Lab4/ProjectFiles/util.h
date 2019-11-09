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

#define SIGGEN_TRIGGER_TIME 10

#define SIG_CMD_READY 0x00000000

#define SIG_INSTANT_CHECK_TIME 0 // O tempo 0 no osSignalWait() retorna imediatamente.

#define SIG_WAVEFORM_SIN 0x00000001
#define SIG_WAVEFORM_TRI 0x00000002
#define SIG_WAVEFORM_TRA 0x00000004
#define SIG_WAVEFORM_SQR 0x00000008
#define SIG_WAVEFORM_SAW 0x00000010

#define SIG_WAVEFORM_CHANGE (\
    SIG_WAVEFORM_SIN |\
    SIG_WAVEFORM_TRI |\
    SIG_WAVEFORM_TRA |\
    SIG_WAVEFORM_SQR |\
    SIG_WAVEFORM_SAW )

#define SIMULADOR 0

typedef enum
{
    SINUSOIDAL,
    TRIANGULAR,
    TRAPEZOIDAL,
    SQUARE,
    SAWTOOTH
} WAVEFORMS;

#endif
