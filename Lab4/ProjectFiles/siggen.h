#ifndef __SIGGEN__
#define __SIGGEN__

#include <math.h>

#include "thread.h"

#include "util.h"

#include "mypwm.h"

#define N 65536.0
#define MAX_AMPLITUDE 3.3


extern WAVEFORMS updateWaveformPWM(WAVEFORMS oldWaveform, WAVEFORMS newWaveform);
extern uint16_t updateFrequencyPWM(float *oldFrequency, float newFrequency, uint16_t oldK);
extern float updateDutyCycle(uint8_t waveform, float amplitude, uint16_t n, uint16_t k);
extern float updateAmplitudePWM(float oldAmplitude, float newAmplitude);
#endif
